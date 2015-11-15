
#include "gpio_i2c.h"
#include "ge.h"
#include <stdlib.h>

struct fab_gpio_i2c {
    fab_gpio_t clk;
    fab_gpio_t dat;
};

fab_gpio_i2c_t fab_gpio_i2c_create(fab_gpio_t clk, fab_gpio_t dat)
{
    struct fab_gpio_i2c *bus = NULL;
   
    FAB_CGE_NULL(bus = malloc(sizeof(struct fab_gpio_i2c)));

    bus->clk = clk;
    bus->dat = dat;

    FAB_CGE_NEG(fab_gpio_set_direction(bus->clk, FAB_GPIO_IN));
    FAB_CGE_NEG(fab_gpio_set_direction(bus->dat, FAB_GPIO_IN));

    return bus;

error:
    if (bus)
        free(bus);

    return NULL;
}

static int fab_gpio_i2c_pull_down(fab_gpio_t gpio)
{
    FAB_CGE_NEG(fab_gpio_set_direction(gpio, FAB_GPIO_OUT));
    FAB_CGE_NEG(fab_gpio_write(gpio, 0));

    return 0;
error:
    return -1;
}


static int fab_gpio_i2c_release(fab_gpio_t gpio)
{
    FAB_CGE_NEG(fab_gpio_set_direction(gpio, FAB_GPIO_IN));

    return 0;

error:
    return -1;
}

static int fab_gpio_i2c_wait_release(fab_gpio_t gpio)
{
    int readvalue;

    do {
        FAB_CGE_NEG(readvalue = fab_gpio_read(gpio));
    } while (readvalue == 0);
    
    return 0;

error:
    return -1;
}

static int fab_gpio_i2c_generate_start_condition(fab_gpio_i2c_t bus)
{
    FAB_CGE_NEG(fab_gpio_i2c_pull_down(bus->dat));
    FAB_CGE_NEG(fab_gpio_i2c_pull_down(bus->clk));

    return 0;

error:
    return -1;
}

static int fab_gpio_i2c_generate_stop_condition(fab_gpio_i2c_t bus)
{
    /* release clock */
    FAB_CGE_NEG(fab_gpio_i2c_release(bus->clk));

    /* wait for client to release clock as well */
    FAB_CGE_NEG(fab_gpio_i2c_wait_release(bus->clk));

    /* release dat */
    FAB_CGE_NEG(fab_gpio_i2c_release(bus->dat));

    return 0;

error:
    return -1;
}



static int fab_gpio_i2c_send_byte(fab_gpio_i2c_t bus, uint8_t byte)
{
    int i;
    int ack;
    /* send address and read/write bit */
    for (i = 0; i < 8; i++) {
        /* set dat */
        if ((byte << i) & 0x80)
            FAB_CGE_NEG(fab_gpio_i2c_release(bus->dat));
        else
            FAB_CGE_NEG(fab_gpio_i2c_pull_down(bus->dat));

        /* release clock */
        FAB_CGE_NEG(fab_gpio_i2c_release(bus->clk));

        /* wait for slave to release clock as well */
        FAB_CGE_NEG(fab_gpio_i2c_wait_release(bus->clk));

        /* pull down clock */
        FAB_CGE_NEG(fab_gpio_i2c_pull_down(bus->clk));
    }

    /* now start reading ack */

    /* release dat */
    FAB_CGE_NEG(fab_gpio_i2c_release(bus->dat));

    /* release clock */
    FAB_CGE_NEG(fab_gpio_i2c_release(bus->clk));

    /* wait for client to release clock as well */
    FAB_CGE_NEG(fab_gpio_i2c_wait_release(bus->clk));

    /* read ack */
    FAB_CGE_NEG(ack = fab_gpio_read(bus->dat));

    /* pull down clock */
    FAB_CGE_NEG(fab_gpio_i2c_pull_down(bus->clk));

    /* pull down dat */
    FAB_CGE_NEG(fab_gpio_i2c_pull_down(bus->clk));


    return ack;

error:
    return -1;
}

int fab_gpio_i2c_transfer(fab_gpio_i2c_t bus, uint8_t address, const char *buf, size_t buflen)
{
    int i;

    address <<= 1;

    FAB_CGE_NEG(fab_gpio_i2c_generate_start_condition(bus));

    FAB_CGE_NEG(fab_gpio_i2c_send_byte(bus, address));

    for (i = 0; i < buflen; i++) {
        FAB_CGE_NEG(fab_gpio_i2c_send_byte(bus, buf[i]));
    }

    FAB_CGE_NEG(fab_gpio_i2c_generate_stop_condition(bus));

    return 0;

error:
    return -1;
}

void fab_gpio_i2c_destroy(fab_gpio_i2c_t bus)
{
    fab_gpio_set_direction(bus->clk, FAB_GPIO_IN);
    fab_gpio_set_direction(bus->dat, FAB_GPIO_IN);

    free(bus);
}

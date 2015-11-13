
#include "gpio_spi.h"

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <ge.h>

#define GET_BIT(data, byte, bit)                ((((uint8_t *) data)[byte]) >> (7 - bit)) & 1
#define SET_BIT(data, byte, bit, value)                     \
do {                                                        \
    if (value) {                                            \
        ((uint8_t *) data)[byte] |=  1 << (7 - bit);        \
    } else {                                                \
        ((uint8_t *) data)[byte] &=  ~(1 << (7 - bit));     \
    }                                                       \
} while(0)


struct fab_gpio_spi {
    fab_gpio_t sclk;
    fab_gpio_t mosi;
    fab_gpio_t miso;
};


fab_gpio_spi_t fab_gpio_spi_create(fab_gpio_t sclk, fab_gpio_t mosi, fab_gpio_t miso)
{
    fab_gpio_spi_t spi;

    FAB_CGE_NULL(spi = malloc(sizeof(struct fab_gpio_spi)));

    spi->sclk = sclk;
    spi->mosi = mosi;
    spi->miso = miso;

    if (fab_gpio_set_direction(sclk, FAB_GPIO_OUT) != 0)
        goto error;

    if (fab_gpio_set_direction(mosi, FAB_GPIO_OUT) != 0)
        goto error;

    if (fab_gpio_set_direction(miso, FAB_GPIO_IN) != 0)
        goto error;

    fab_gpio_write(sclk, 0);

    return spi;

error:
    if (spi != NULL)
        free(spi);

    return NULL;
}


int fab_gpio_spi_transfer(fab_gpio_spi_t spi, const void *out, void *in, size_t len)
{
    int byte, bit;
    int bitvalue;
      
    for (byte = 0; byte < len; byte++) {
        for (bit = 0; bit < 8; bit++)  {

            if (fab_gpio_write(spi->mosi, GET_BIT(out, byte, bit)) == -1)
                goto error;

            usleep(100);
            if (fab_gpio_write(spi->sclk, 1) == -1)
                goto error;

            usleep(100);

            if ((bitvalue = fab_gpio_read(spi->miso)) == -1)
                goto error;

            SET_BIT(in, byte, bit, bitvalue);

            if (fab_gpio_write(spi->sclk, 0) == -1)
                goto error;
        }
    }

    return 0;

error:
    return -1;
}


void fab_gpio_spi_destroy(fab_gpio_spi_t spi)
{
    free(spi);
}

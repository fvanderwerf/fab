
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "gpio_test.h"

#include <gpio.h>

void *data_ptr = (void *) 0x1234;

struct fab_gpio gpio;

static int return_value;

static int destroy_called;

static void gpio_destroy(void *data)
{
    ck_assert_ptr_eq(data_ptr, data);
    destroy_called++;
}

static enum fab_gpio_direction expected_direction;
static int gpio_set_direction(void *data, enum fab_gpio_direction direction)
{
    ck_assert_ptr_eq(data_ptr, data);
    ck_assert(expected_direction == direction);

    return return_value;
}

static enum fab_gpio_direction return_direction;
static enum fab_gpio_direction gpio_get_direction(void *data)
{
    ck_assert_ptr_eq(data_ptr, data);

    return return_direction;
}

static int gpio_read(void *data)
{
    ck_assert_ptr_eq(data_ptr, data);
    return return_value;
}

static int expected_write;
static int gpio_write(void *data, int value)
{
    ck_assert_ptr_eq(data_ptr, data);
    ck_assert_int_eq(expected_write, value);

    return return_value;
}

void test_gpio_destroy()
{
    destroy_called = 0;
    gpio.destroy = gpio_destroy;
    gpio.data = data_ptr;

    fab_gpio_destroy(&gpio);

    ck_assert_int_eq(1, destroy_called);
}

void test_gpio_set_direction()
{
    gpio.set_direction = gpio_set_direction;
    gpio.data = data_ptr;

    return_value = 42;
    ck_assert_int_eq(42, fab_gpio_set_direction(&gpio, expected_direction = FAB_GPIO_IN));
   
    return_value = 24;
    ck_assert_int_eq(24, fab_gpio_set_direction(&gpio, expected_direction = FAB_GPIO_OUT));
}


void test_gpio_get_direction()
{
    gpio.get_direction = gpio_get_direction;
    gpio.data = data_ptr;

    return_direction = FAB_GPIO_IN;

    ck_assert(FAB_GPIO_IN == fab_gpio_get_direction(&gpio));

    return_direction = FAB_GPIO_OUT;

    ck_assert(FAB_GPIO_OUT == fab_gpio_get_direction(&gpio));

}

void test_gpio_read()
{
    gpio.read = gpio_read;
    gpio.data = data_ptr;

    return_value = 35;
    ck_assert_int_eq(return_value, fab_gpio_read(&gpio));
}


void test_gpio_write()
{
    gpio.write = gpio_write;
    gpio.data = data_ptr;

    return_value = 27;
    expected_write = 67;
    ck_assert_int_eq(27 , fab_gpio_write(&gpio, 67));
}


Suite *gpio_test_suite()
{
    Suite *s;
    TCase *destroy, *setdir, *getdir, *read, *write;

    s = suite_create("gpio");

    destroy = tcase_create("destroy");
    tcase_add_test(destroy, test_gpio_destroy);
    suite_add_tcase(s, destroy);

    setdir = tcase_create("set_direction");
    tcase_add_test(setdir, test_gpio_set_direction);
    suite_add_tcase(s, setdir);

    getdir = tcase_create("get_direction");
    tcase_add_test(getdir, test_gpio_get_direction);
    suite_add_tcase(s, getdir);

    read = tcase_create("read");
    tcase_add_test(read, test_gpio_read);
    suite_add_tcase(s, read);

    write = tcase_create("write");
    tcase_add_test(write, test_gpio_write);
    suite_add_tcase(s, write);

    return s;
}

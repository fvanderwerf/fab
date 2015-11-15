
#ifndef FAB_GPIO_I2C_H
#define FAB_GPIO_I2C_H

#include "gpio.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct fab_gpio_i2c *fab_gpio_i2c_t;

fab_gpio_i2c_t fab_gpio_i2c_create(fab_gpio_t clk, fab_gpio_t dat);

int fab_gpio_i2c_transfer(fab_gpio_i2c_t bus, uint8_t address, const char *buf, size_t buflen);

void fab_gpio_i2c_destroy(fab_gpio_i2c_t bus);

#endif /* FAB_GPIO_I2C_H */

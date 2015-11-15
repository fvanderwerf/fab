
#ifndef FABGPIO_SPI_H
#define FAB_GPIO_SPI_H

#include "gpio.h"
#include <stddef.h>

typedef struct fab_gpio_spi *fab_gpio_spi_t;

fab_gpio_spi_t fab_gpio_spi_create(fab_gpio_t sclk, fab_gpio_t mosi, fab_gpio_t miso);

int fab_gpio_spi_transfer(fab_gpio_spi_t spi, const void *out, void *in, size_t len);

void fab_gpio_spi_destroy(fab_gpio_spi_t spi);

#endif /* FAB_GPIO_SPI_H */


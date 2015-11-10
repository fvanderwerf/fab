
#ifndef FAB_GPIO_H
#define FAB_GPIO_H

enum fab_gpio_direction
{
    FAB_GPIO_IN,
    FAB_GPIO_OUT
};

typedef struct fab_gpio *fab_gpio_t;

typedef void (*fab_gpio_destroy_t)(void *data);
typedef enum fab_gpio_direction (*fab_gpio_get_direction_t)(void *data);
typedef int (*fab_gpio_set_direction_t)(void *data, enum fab_gpio_direction direction);
typedef int  (*fab_gpio_read_t)(void *data);
typedef int (*fab_gpio_write_t)(void *data, int value);

struct fab_gpio {
    fab_gpio_destroy_t destroy;
    fab_gpio_get_direction_t get_direction;
    fab_gpio_set_direction_t set_direction;
    fab_gpio_read_t read;
    fab_gpio_write_t write;

    void *data;
};


static inline void fab_gpio_destroy(fab_gpio_t gpio)
{
    gpio->destroy(gpio->data);
}

static inline enum fab_gpio_direction fab_gpio_get_direction(fab_gpio_t gpio)
{
    return gpio->get_direction(gpio->data);
}

static inline int fab_gpio_set_direction(fab_gpio_t gpio, enum fab_gpio_direction direction)
{
    return gpio->set_direction(gpio->data, direction);
}

static inline int fab_gpio_read(fab_gpio_t gpio)
{
    return gpio->read(gpio->data);
}

static inline int fab_gpio_write(fab_gpio_t gpio, int value)
{
    return gpio->write(gpio->data, value);
}

#endif /* FAB_GPIO_H */


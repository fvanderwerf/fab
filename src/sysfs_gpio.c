
#include "sysfs_gpio.h"

#include "ge.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_INT_STRING_LEN sizeof(int) * 3 + 2

static const char sysfs_gpio_export_filename[] = "/sys/class/gpio/export";
static const char sysfs_gpio_unexport_filename[] = "/sys/class/gpio/unexport";

static const char sysfs_gpio_direction_filename_format[] = "/sys/class/gpio/gpio%d/direction";
static int sysfs_gpio_direction_filename_maxlen = sizeof(sysfs_gpio_direction_filename_format) + MAX_INT_STRING_LEN;

static const char sysfs_gpio_value_filename_format[] = "/sys/class/gpio/gpio%d/value";
static int sysfs_gpio_value_filename_maxlen = sizeof(sysfs_gpio_value_filename_format) + MAX_INT_STRING_LEN;

struct sysfs_gpio {
    struct fab_gpio gpio;

    int gpio_num;
    enum fab_gpio_direction direction;
};

static int sysfs_gpio_write_number(const char *filename, int number)
{
    FILE *export = NULL;
    int byteswritten;

    FAB_CGE_NULL(export = fopen(filename, "w"));

    FAB_CGE_NEG(fprintf(export, "%d", number));

    fclose(export);
    return 0;

error:
    perror(filename);
    if (export != NULL)
        fclose(export);

    return -1;
}


static int sysfs_gpio_export(int gpio_num)
{
    return sysfs_gpio_write_number(sysfs_gpio_export_filename, gpio_num);
}

static int sysfs_gpio_unexport(int gpio_num)
{
    return sysfs_gpio_write_number(sysfs_gpio_unexport_filename, gpio_num);
}


static void sysfs_gpio_setup_gpio(struct sysfs_gpio *gpio)
{
    gpio->gpio.data = gpio;
    gpio->gpio.destroy = (fab_gpio_destroy_t) sysfs_gpio_destroy;
    gpio->gpio.get_direction = (fab_gpio_get_direction_t) sysfs_gpio_get_direction;
    gpio->gpio.set_direction = (fab_gpio_set_direction_t) sysfs_gpio_set_direction;
    gpio->gpio.read = (fab_gpio_read_t) sysfs_gpio_read;
    gpio->gpio.write = (fab_gpio_write_t) sysfs_gpio_write;
}

sysfs_gpio_t sysfs_gpio_create(int gpio_num)
{
    struct sysfs_gpio *instance = NULL;

    FAB_CGE_NULL(instance = malloc(sizeof(*instance)));

    instance->gpio_num = gpio_num;
    instance->direction = FAB_GPIO_IN;

    FAB_CGE_NEG(sysfs_gpio_export(gpio_num) != 0);

    sysfs_gpio_set_direction(instance, FAB_GPIO_IN);

    sysfs_gpio_setup_gpio(instance);

    return instance;
error:
    if (instance != NULL)
        free(instance);

    return NULL;
}

fab_gpio_t sysfs_gpio_to_gpio(sysfs_gpio_t gpio)
{
    return &(gpio->gpio);
}

enum fab_gpio_direction sysfs_gpio_get_direction(sysfs_gpio_t gpio)
{
    return gpio->direction;
}

static const char *sysfs_gpio_get_direction_string(enum fab_gpio_direction direction)
{
    switch (direction) {
        case FAB_GPIO_IN:
            return "in";
            break;
        case FAB_GPIO_OUT:
            return "out";
            break;
        default:
            errno = EINVAL;
            return NULL;
    }
}

int sysfs_gpio_set_direction(sysfs_gpio_t gpio, enum fab_gpio_direction direction)
{
    FILE *fdirection;
    int numbytes_written;

    char filename[sysfs_gpio_direction_filename_maxlen];
    sprintf(filename, sysfs_gpio_direction_filename_format, gpio->gpio_num);

    FAB_CGE_NULL(fdirection = fopen(filename, "w"));

    numbytes_written = fputs(sysfs_gpio_get_direction_string(direction), fdirection);
    FAB_CGE(numbytes_written == EOF);

    gpio->direction = direction;

    fclose(fdirection);
    return 0;

error:
    perror(filename);

    if (fdirection != NULL)
        fclose(fdirection);

    return -1;
}

int sysfs_gpio_read(sysfs_gpio_t gpio)
{
    char filename[sysfs_gpio_value_filename_maxlen];
    FILE *valuefile = NULL;
    char value[1];
    int numelems_read;
    int ret;

    sprintf(filename, sysfs_gpio_value_filename_format, gpio->gpio_num);

    FAB_CGE_NULL(valuefile = fopen(filename, "r"));

    numelems_read = fread(value, 1, 1, valuefile);
    FAB_CGE(numelems_read != 1);

    fclose(valuefile);

    if (value[0] == '0')
        ret = 0;
    else
        ret = 1;

    return ret;

error:
    if (valuefile != NULL)
        fclose(valuefile);

    return -1;
}

int sysfs_gpio_write(sysfs_gpio_t gpio, int value)
{
    char filename[sysfs_gpio_value_filename_maxlen];
    FILE *valuefile = NULL;
    int numbytes_written;

    FAB_CGE_ERRNO(gpio->direction != FAB_GPIO_OUT, EPERM);

    sprintf(filename, sysfs_gpio_value_filename_format, gpio->gpio_num);

    FAB_CGE_NULL(valuefile = fopen(filename, "w"));

    FAB_CGE_NEG(numbytes_written = fprintf(valuefile, "%d", value));

    fclose(valuefile);

    return 0;

error:

    if (valuefile != NULL)
        fclose(valuefile);

    return -1;
}

void sysfs_gpio_destroy(sysfs_gpio_t gpio)
{
    sysfs_gpio_set_direction(gpio, FAB_GPIO_IN);
    sysfs_gpio_unexport(gpio->gpio_num);
    free(gpio);
}

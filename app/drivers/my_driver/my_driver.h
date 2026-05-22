#ifndef MY_DRIVER_H
#define MY_DRIVER_H
#include <zephyr/drivers/sensor.h>

typedef int (*my_driver_set_param_t)(const struct device *dev, int param);

struct my_extended_api{
    struct sensor_driver_api sensor_api;
    my_driver_set_param_t extension_func;
};

static inline int my_driver_set_param(const struct device *dev,int param)
{
    const struct my_extended_api *api = (const struct my_extended_api*)dev->api;

    return api->extension_func(dev,param);
}


#endif
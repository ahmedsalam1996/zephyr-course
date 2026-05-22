#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include "my_driver.h"

#define DT_DRV_COMPAT my_driver

LOG_MODULE_REGISTER(my_driver,LOG_LEVEL_INF);


struct my_extended_param{
    int param;
};

static struct my_extended_param my_param;
static int channel_get_my_sensor(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val){
                        return 0;
}

static int fetch_my_sensor(const struct device *dev,
				     enum sensor_channel chan){
                        return 1;
                     }

static int set_param_my_sensor(const struct device *dev,int input_param){
    struct my_extended_param *ex_param = (struct my_extended_param*)dev->data;
    ex_param->param = input_param;
    LOG_INF("parameter updated to %d",ex_param->param);
    return 0;
}

/*static DEVICE_API(sensor,api_my_sensor) = {
    .channel_get = channel_get_my_sensor,
    .sample_fetch = fetch_my_sensor,
    .extension_func = set_param_my_sensor,
};*/

static const struct my_extended_api api_my_sensor = {
    .sensor_api = {
        .channel_get = channel_get_my_sensor,
        .sample_fetch = fetch_my_sensor,
    },
    .extension_func = set_param_my_sensor,
};

static int init(const struct device* dev){
    struct my_extended_param *ex_param = (struct my_extended_param*)dev->data;
    ex_param->param = 0;
    LOG_INF("my extended driver initialized!");
    return 0;
}

DEVICE_DT_INST_DEFINE(0,init,NULL,&my_param,NULL,POST_KERNEL,80,&api_my_sensor);
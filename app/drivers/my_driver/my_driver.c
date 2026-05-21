#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT my_driver

LOG_MODULE_REGISTER(my_driver,LOG_LEVEL_INF);

static int channel_get_my_sensor(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val){
                        return 0;
}

static int fetch_my_sensor(const struct device *dev,
				     enum sensor_channel chan){
                        return 1;
                     }



static DEVICE_API(sensor,api_my_sensor) = {
    .channel_get = channel_get_my_sensor,
    .sample_fetch = fetch_my_sensor,
};

static int init(const struct device* dev){
    LOG_INF("my driver initialized!");
    return 0;
}

DEVICE_DT_INST_DEFINE(0,init,NULL,NULL,NULL,POST_KERNEL,80,&api_my_sensor);
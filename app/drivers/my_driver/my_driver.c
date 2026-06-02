#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include "my_driver.h"
#include <stdlib.h>
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


static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = DEVICE_DT_INST_GET(0);
    if (!device_is_ready(dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }

   
    int ret = sensor_sample_fetch(dev);
    shell_print(sh, "sensor_sample_fetch() returned: %d", ret);
    return ret;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = DEVICE_DT_INST_GET(0);
    if (!device_is_ready(dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }

    struct sensor_value val = {0};
    
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    
    shell_print(sh, "sensor_channel_get() returned: %d | Val: %d.%06d", 
                ret, val.val1, val.val2);
    return ret;
}


static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = DEVICE_DT_INST_GET(0);
    bool ready = device_is_ready(dev);

    shell_print(sh, "Device Name: %s", dev->name);
    shell_print(sh, "Ready State: %s", ready ? "READY" : "NOT READY");
    return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = DEVICE_DT_INST_GET(0);
    if (!device_is_ready(dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }

    //converting string to int
    char *endptr;
    long val = strtol(argv[1], &endptr, 10);

    //checking for valid number
    if (*endptr != '\0') {
        shell_error(sh, "Error: Invalid number format '%s'", argv[1]);
        return -EINVAL;
    }

    
    if (val < 0 || val > 255) {
        shell_error(sh, "Error: Value %ld out of range (0-255)", val);
        return -EINVAL;
    }

    int ret = my_driver_set_param(dev, (int)val);
    shell_print(sh, "Parameter successfully set to %ld", val);
    
    return ret;
}
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor_driver,
    SHELL_CMD_ARG(fetch, NULL, "Call sensor_sample_fetch()", cmd_sensor_fetch, 1, 0),
    SHELL_CMD_ARG(read,  NULL, "Call sensor_channel_get() and print result", cmd_sensor_read, 1, 0),
    SHELL_CMD_ARG(info,  NULL, "Print device name and ready state", cmd_sensor_info, 1, 0),
    SHELL_CMD_ARG(set,   NULL, "Set extension parameter: set <value>", cmd_sensor_set, 2, 0),
    SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor_driver, &sub_sensor_driver, "Sensor driver shell commands", NULL);

DEVICE_DT_INST_DEFINE(0,init,NULL,&my_param,NULL,POST_KERNEL,80,&api_my_sensor);
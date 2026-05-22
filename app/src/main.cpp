#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

extern "C"{
    #include "my_driver/my_driver.h"
}
//#define SLEEP_TIME_MS 100

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)
//#define LED_NODE2 DT_NODELABEL(blue_led)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);
const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(my_driver1));
int main(void)
{
    bool led_state = true;
    int data = 10;
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    my_driver_set_param(driver,data);
    while (1) {
        data++;
        led_state = sensor_sample_fetch(driver);
        if (gpio_pin_set_dt(&led,led_state) < 0) return 0;
        LOG_INF("LED state: %s","ON");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        led_state = sensor_channel_get(driver,SENSOR_CHAN_ALL,NULL);
        if (gpio_pin_set_dt(&led,led_state) < 0) return 0;
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        LOG_INF("LED state: %s","OFF");
        my_driver_set_param(driver,data);
    }
    return 0;
}

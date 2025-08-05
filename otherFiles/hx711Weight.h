#ifndef __HX711__H__
#define __HX711__H__

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>

#include <sensor/hx711/hx711.h>
#include <stddef.h>




class Hx711 
{
public:
    Hx711(const struct device *hx711_dev);
    void set_rate(enum hx711_rate rate);
    struct sensor_value measure(void);

private:

    const struct device *hx711_dev;

};


#endif
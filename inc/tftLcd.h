#ifndef __TFT_LCD__H__
#define __TFT_LCD__H__



#include <zephyr/device.h>
// #include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
// #include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <zephyr/logging/log.h>
// #include <stdio.h>
// #include <string.h>
#include <zephyr/kernel.h>
// #include <lvgl_input_device.h>
// #include <lv_image.h>


class TftLcd
{
public:
    TftLcd(const struct device *dev);

private:
    const struct device *displayDev;

};



#endif 
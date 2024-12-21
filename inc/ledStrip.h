#ifndef __LED_STRIP__
#define __LED_STRIP__

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>

#define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }

class LedStrip
{
public:
    LedStrip(const struct device *_strip, uint8_t _numPixels);
    int update(struct led_rgb &rgb, uint8_t number);

private: 
    const struct device *strip; 
    uint8_t numPixels;
    struct led_rgb *pixels;

};



#endif
#ifndef __LED_STRIP__
#define __LED_STRIP__

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/led_strip.h>


class LedStrip
{
public:
    LedStrip(const struct device *_strip, uint8_t _numPixels);
    void update(struct led_rgb &pixels);

private: 
    const struct device *strip; 
    uint8_t numPixels;

};



#endif
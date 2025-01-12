#ifndef __DISPLAY24__H__
#define __DISPLAY24__H__

#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>

extern const uint8_t segment_map[128];

extern "C" int reg_595_port_set_masked_raw(const struct device *dev, uint8_t *mask, uint8_t *val);

extern "C" int reg_595_port_clear_bits_raw(const struct device *dev, uint8_t *mask);
class Display24
{
public:
    Display24(const struct gpio_dt_spec *_display);

    void displayStr(const char * _str, uint8_t pos);
private:
    struct k_timer displayTimer;

    const struct gpio_dt_spec *display;
    char str[24] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t strLen = 0;

    uint8_t mask[24] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
};
#endif
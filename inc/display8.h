#ifndef __DISPLAY8__H__
#define __DISPLAY8__H__

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

class Display8
{
public:
    Display8(const struct gpio_dt_spec *_display);

    void displayStr(const char * _str);
private:
    struct k_timer displayTimer;
    static void displayTimerHandler(struct k_timer *timer);

    struct k_work displayWork;
    static void displayWorkHandler(struct k_work *work);
    void displayChar(uint8_t pos, char c);
    const struct gpio_dt_spec *display;
    void displayRefresh();
    char str[9];
    uint8_t strLen = 0;
};





#endif
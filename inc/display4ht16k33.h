
#ifndef __DISPLAY4_HT_16_K33__H__
#define __DISPLAY4_HT_16_K33__H__



#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include "topics.h"
#include <zephyr/input/input.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include "puzzle.h"
#include <zephyr/drivers/led.h>
extern const uint8_t segment_map[128];

class Display4
{
public:
    Display4(const struct device *const _display, bool blink);
    void displayStr(char *);
    void setBrightness();
    void displayChar(char c, uint8_t pos);

    struct k_timer blinkTimer;
    static void blinkTimerHandler(struct k_timer *timer);

    struct k_work blinkWork;
    static void blinkWorkHandler(struct k_work *work);

private:
    const struct device *display;
    bool blink = false;
    
};

#endif 


#ifndef __DISPLAY8_HT_16_K33__H__
#define __DISPLAY8_HT_16_K33__H__



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

class Display8
{
public:
    Display8(const struct device *const _display);
    void displayStr(char *);
    void setBrightness();
    void displayChar(char c, uint8_t pos);

private:
    const struct device *display;
    
};

#endif 
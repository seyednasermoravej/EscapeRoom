
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

class Display4
{
public:
    Display4(const struct device *const _display): display(_display){}
    void print(char *);
    void setBrightness();

private:
    const struct device *const display;

};

#endif 
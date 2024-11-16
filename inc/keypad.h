#ifndef __KEYPAD__H__
#define __KEYPAD__H__


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include "zephyr/logging/log.h"
#include "messageQueues.h"

#ifdef __cplusplus
extern "C" {
#endif


int keypad(const struct device *dev, char *topic);


#ifdef __cplusplus
}
#endif
#endif
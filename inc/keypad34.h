#ifndef __KEYPAD34__H__
#define __KEYPAD34__H__


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include "zephyr/logging/log.h"
#include "messageQueues.h"


class Keypad34
{
public:
    Keypad34(const char * _prefix);
    void static handlerWrapper(struct input_event *val, void*);
    void handler(struct input_event*);
private:
    const char *prefix;
};

#endif
#ifndef __DOOR_KEYPAD__H__
#define __DOOR_KEYPAD__H__



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
#include "display8.h"

class DoorKeypad: public Puzzle 
{
public:
    DoorKeypad(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    void buttonsHandler(struct input_event *val);
    static void buttonsHandlerWrapper(struct input_event *val, void *userData);
private:
    Display8 *display8;
};

#endif 
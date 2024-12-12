#ifndef __DRAWERS__H__
#define __DRAWERS__H__



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

class Drawers: public Puzzle 
{
public:
    Drawers(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    static void buttonsHandlerWrapper(struct input_event *val, void* userData);
    void buttonsHandler(struct input_event *val);

private:
    size_t startRelaysCommand;



};

#endif 
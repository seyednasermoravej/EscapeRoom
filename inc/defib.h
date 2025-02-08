#ifndef __DEFIB__H__
#define __DEFIB__H__



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

class Defib: public Puzzle 
{
public:
    Defib(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList() override;
    static void buttonsHandlerWrapper(struct input_event *val, void* userData);
    void buttonsHandler(struct input_event *val);

private:
    size_t startRelaysCommand;



};

#endif 
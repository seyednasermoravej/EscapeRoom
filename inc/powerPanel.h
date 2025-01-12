#ifndef __POWER_PANEL__H__
#define __POWER_PANEL__H__



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
#include "servos.h"
#include "display24.h"
#include "display4ht16k33.h"

class PowerPanel: public Puzzle 
{
public:
    PowerPanel(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    static void switchesHandlerWrapper(struct input_event *val, void* userData);
    void switchesHandler(struct input_event *val);

private:
    Servos *servos;
    Display24 *display24;
    Display4 *display4;
};

#endif 
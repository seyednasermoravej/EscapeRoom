
#ifndef __ENTRANCE_DOOR__H__
#define __ENTRANCE_DOOR__H__



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
#include "Wiegand.h"

class EntranceDoor: public Puzzle 
{
public:
    EntranceDoor(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    struct k_timer wiegandTimer;
    static void wiegandTimerHandler(struct k_timer *timer);

private:
    WIEGAND *wg;


};

#endif 
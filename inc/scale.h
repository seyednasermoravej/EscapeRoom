#ifndef __SCALE__H__
#define __SCALE__H__

///////////////////hx711
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>
#include <sensor/hx711/hx711.h>
#include <stddef.h>
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include "topics.h"
#include <zephyr/input/input.h>
#include <zephyr/drivers/gpio.h>
#include "puzzle.h"
#include "hx711Weight.h"


class Scale: public Puzzle 
{
public:
    Scale(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    struct k_timer loadcellTimer;
    static void loadcellTimerHandler(struct k_timer *timer);
    struct sensor_value weight;

private:
    Hx711 *hx711;
};



#endif 
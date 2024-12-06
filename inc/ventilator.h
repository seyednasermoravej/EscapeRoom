#ifndef __VENTILATOR__H__
#define __VENTILATOR__H__



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
#include "adcs.h"

class Ventilator: public Puzzle 
{
public:
    Ventilator(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    uint16_t readAdc(uint8_t channel);
    struct k_timer analogTimer;
    static void analogTimerHandler(struct k_timer *timer);
    void updateAnalog();
    struct k_work analogWork;
    static void analogWorkHanlder(struct k_work *work);

private:
    Adcs *adcs;
    uint16_t analog[4];


};

#endif 
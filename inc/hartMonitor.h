#ifndef __HART_MONITOR__H__
#define __HART_MONITOR__H__



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

class HartMonitor: public Puzzle 
{
public:
    HartMonitor(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    uint16_t readAdc(uint8_t channel);

private:
    Adcs *adcs;


};

#endif 
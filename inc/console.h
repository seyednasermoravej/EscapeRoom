#ifndef __CCONSOLE__H__
#define __CCONSOLE__H__



#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include "topics.h"
#include "lcd.h"
#include <zephyr/input/input.h>
#include <zephyr/drivers/led.h>
#include "main.h"
#include "puzzle.h"

#include <zephyr/dt-bindings/input/input-event-codes.h>

class Console: public Puzzle
{
public:
    Console(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList() override;



private:
    Lcd *lcd1;
    Lcd *lcd2;
};

#endif
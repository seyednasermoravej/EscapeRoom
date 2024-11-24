#ifndef __CONFIG_DEVICE__H__
#define __CONFIG_DEVICE__H__



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

#include <zephyr/dt-bindings/input/input-event-codes.h>

class ConfigDevice
{
public:
    ConfigDevice();
    void messageHandler(struct MqttMsg *msg);


private:
    Lcd *lcd1;
    Lcd *lcd2;
};

#endif
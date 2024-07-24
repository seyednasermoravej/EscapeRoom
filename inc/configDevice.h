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

#include "topics.h"
#include "lcd.h"

extern void lcdThreadCreate();

class ConfigDevice
{
public:
    ConfigDevice();
    void messageHandler(struct MqttMsg *msg);


private:
    int inputsInit();
};

#endif
#ifndef __LABORATORY__H__
#define __LABORATORY__H__


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
#include "ledStrip.h"
#include "Adafruit_TCS34725.h"


class Laboratory
{
public:
    Laboratory();
    void messageHandler(struct MqttMsg *msg);


private:
    LedStrip *ledStrip;
    Adafruit_TCS34725 *tcs;
};



#endif
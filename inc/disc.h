#ifndef __DISC__H__
#define __DISC__H__

#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>

#include "topics.h"
#include "aasdDriver.h"

class Disc
{
public:
    Disc();
    void messageHandler(struct MqttMsg *msg);

private:
    Aasd *aasd;

};











#endif
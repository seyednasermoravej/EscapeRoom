#ifndef __NUMBERS_GUESSING_H__
#define __NUMBERS_GUESSING_H__

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
#include "ledandkeys.h"


extern void ledandkeysThreadCreate();


class NumbersGuessing
{

public:
    NumbersGuessing();
    void messageHandler(struct MqttMsg *msg);

private:
    void sendButtonsVal();
    void segementIncrement();

};



#endif
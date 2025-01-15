#ifndef __GATE__H__
#define __GATE__H__

#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include "lcd.h"
#include "topics.h"

extern void rfidInThreadCreate();

class Gate
{

public:
    Gate();
    void messageHandler(struct MqttMsg *msg);
private:
    void sendDevAddrVal();
    int addrKeysInit();
    int addrKeysVal();
    int hintButtonInit();
    int magLocksInit();
    int relaysCheckInit();

    Lcd *lcd1;
    Lcd *lcd2;

};



#endif
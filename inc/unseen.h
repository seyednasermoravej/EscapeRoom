#ifndef __UNSEEN__H__
#define __UNSEEN__H__



#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
// #include <zephyr/input/input_kbd_matrix.h>

class Unseen
{

public:
    Unseen();
    void messageHandler(struct MqttMsg *msg);

private:


};

#endif 
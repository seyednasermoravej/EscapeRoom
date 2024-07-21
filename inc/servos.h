#ifndef __SERVOS__H__
#define __SERVOS__H__

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



#define STEP PWM_USEC(100)
#define MAX_NUMBER_OF_SERVO_MOTORS          8

class Servos
{
public:
    Servos();
    void messageHandler(struct MqttMsg *msg);

private:

};

#endif

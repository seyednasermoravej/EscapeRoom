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
#include "utils.h"

class Servos
{
public:
    Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos, uint32_t minPulse, uint32_t maxPulse, uint16_t maxDegrees);
    // Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos);
    void move(uint8_t index, uint32_t degrees);
private:
    const struct pwm_dt_spec *servos;
    uint8_t numOfservos;
    uint32_t minPulse;
    uint32_t maxPulse;
    uint16_t maxDegrees;
    float coeff;
};

#endif

#ifndef __colorTubes__H__
#define __colorTubes__H__



#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include "puzzle.h"
#include "Adafruit_TCS34725.h"

class colorTubes: public Puzzle 
{
public:
    colorTubes(const char * room, const char *type, uint8_t numRGBsensors);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;

    struct k_timer rgbSensorTimer;
    struct k_work rgbSensorWork;
    static void rgbSensorTimerHandler(struct k_timer *timer);
    static void rgbSensorWorkHandler(struct k_work *work);

    struct MqttMsg msgReader = {0};
private:
    Adafruit_TCS34725 **rgbsensors;
    uint8_t numRGBsensors; 
};

#endif 
#ifndef __PUZZLE__H__
#define __PUZZLE__H__



#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include "topics.h"
#include <zephyr/input/input.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/mqtt.h>
#include <stdlib.h>


class Puzzle 
{
protected:
    char *roomName;
    char *puzzleTypeName;
    struct mqtt_topic mqttList[20];
    uint16_t mqttCount;
    virtual void creatingMqttList(uint16_t mqttCount) = 0;
    virtual void test();
    struct k_timer aliveTimer;
    mqtt_topic *createMqttTopic(const char *topicName);

public:
    // Constructor to initialize roomName and puzzleTypeName
    Puzzle(const char* room, const char* type);

    // Virtual method to be implemented by subclasses

    // Common method for all subclasses
    static void alive(struct k_timer *);
    virtual void messageHandler(MqttMsg *msg) = 0;

    uint16_t getMqttCount();
    struct mqtt_topic *getMqttList();
    // Virtual destructor for proper cleanup of derived classes
    virtual ~Puzzle(); 
};

#endif
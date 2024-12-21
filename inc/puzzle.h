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
#include <string.h>
#include <zephyr/drivers/led_strip.h>


class Puzzle 
{
protected:
    char *roomName;
    char *puzzleTypeName;
    struct mqtt_topic mqttList[20];
    uint16_t mqttCount = 0;
    virtual void creatingMqttList(uint16_t mqttCount) = 0;
    virtual void test();
    struct k_timer aliveTimer;
    mqtt_topic *createMqttTopic(const char *topicName);
    char mqttCommand[128];
    int validTopic(char *topic, char *command);
    int peripheralIdx(const char *field, char *command);
    int relayOperation(char *command, const gpio_dt_spec *relay, bool momentry);
    struct led_rgb retrieveColors(char *str);
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
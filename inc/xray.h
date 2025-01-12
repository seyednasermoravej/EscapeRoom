#ifndef __XRAY__H__
#define __XRAY__H__



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
#include "rfidIn.h"
#include "main.h"

class Xray: public Puzzle 
{
public:
    Xray(const char * room, const char *type, uint8_t numRfids);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    struct k_timer cardsReaderTimer;
    struct k_work cardsReaderWork;
    static void cardsReaderTimerHandler(struct k_timer *timer);
    static void cardsReaderWorkHandler(struct k_work *work);

    struct MqttMsg msgReader = {0};
private:
    Adafruit_PN532 **rfids;
    uint8_t numRfids; 



};

#endif 
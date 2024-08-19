#ifndef __QUEUE__H__
#define __QUEUE__H__

// #include <mqueue.h>
#include "zephyr/kernel.h"

#ifdef __cplusplus
extern "C" {
#endif
// extern mqd_t msqReceivedFromMQTT;
// extern mqd_t msqSendToMQTT;
// extern mqd_t msqRfid730;
// extern mqd_t msqRfidRc;
// extern mqd_t msqLcd;

extern struct k_msgq msqReceivedFromMQTT;
extern struct k_msgq msqSendToMQTT;
extern struct k_msgq msqRfidOut;
extern struct k_msgq msqRfidIn;
extern struct k_msgq msqLcd1;
extern struct k_msgq msqLcd2;
extern struct k_msgq msqkeypad;
extern struct k_msgq msqLedandKey;

#define         MESSAGE_QUEUE_LEN       128
#define         TOPIC_LEN               128
#define         UUID_LEN                7
struct MqttMsg
{
    char topic[TOPIC_LEN];
    char msg[MESSAGE_QUEUE_LEN];
};

struct LcdMsg
{
    char firstLine[MESSAGE_QUEUE_LEN];
    char secondLine[MESSAGE_QUEUE_LEN];
};


// struct RfidMsg
// {
//     char topic[TOPIC_LEN];
//     char uuid[UUID_LEN];
// };

// int messageQueueInit();

#ifdef __cplusplus
}
#endif
#endif
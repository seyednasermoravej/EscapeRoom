#ifndef __QUEUE__H__
#define __QUEUE__H__

#include <mqueue.h>

#ifdef __cplusplus
extern "C" {
#endif
extern mqd_t msqReceivedFromMQTT;
extern mqd_t msqSendToMQTT;
extern mqd_t msqRfid730;
extern mqd_t msqRfidRc;
extern mqd_t msqLcd;

int messageQueueInit();

#ifdef __cplusplus
}
#endif
#endif
#include "messageQueues.h"



mqd_t msqReceivedFromMQTT;
mqd_t msqSendToMQTT;
mqd_t msqRfid730;
mqd_t msqRfidRc;
mqd_t msqLcd;

int messageQueueInit()
{ 
    msqReceivedFromMQTT = mq_open("/msqReceivedFromMQTT", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
    msqSendToMQTT = mq_open("/msqSendToMQTT", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
    msqRfid730 = mq_open("/msqRfid730", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
    msqRfidRc = mq_open("/msqRfidRc", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
    msqLcd = mq_open("/msqLcd", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
}
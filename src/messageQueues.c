#include "messageQueues.h"



struct k_msgq msqReceivedFromMQTT;
struct k_msgq msqSendToMQTT;
struct k_msgq msqRfid730;
struct k_msgq msqRfidRc;
struct k_msgq msqLcd;



K_MSGQ_DEFINE(msqReceivedFromMQTT, sizeof(struct MqttMsg), 8, 4);
K_MSGQ_DEFINE(msqSendToMQTT, sizeof(struct MqttMsg), 8, 4);

// int messageQueueInit()
// {
//     msqReceivedFromMQTT = mq_open("/msqReceivedFromMQTT", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
//     msqSendToMQTT = mq_open("/msqSendToMQTT", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
//     msqRfid730 = mq_open("/msqRfid730", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
//     msqRfidRc = mq_open("/msqRfidRc", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
//     msqLcd = mq_open("/msqLcd", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
// }
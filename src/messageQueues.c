#include "messageQueues.h"



struct k_msgq msqReceivedFromMQTT;
struct k_msgq msqSendToMQTT;
// struct k_msgq msqLcd1;
// struct k_msgq msqLcd2;
// struct k_msgq msqKeypad;
struct k_msgq msqLedandKey;

K_MSGQ_DEFINE(msqReceivedFromMQTT, sizeof(struct MqttMsg), 8, 4);
K_MSGQ_DEFINE(msqSendToMQTT, sizeof(struct MqttMsg), 8, 4);
// K_MSGQ_DEFINE(msqLcd1, sizeof(struct LcdMsg), 8, 4);
// K_MSGQ_DEFINE(msqLcd2, sizeof(struct LcdMsg), 8, 4);
/////////////////////bug sizeof(strut LcdMsg) for led and keys is bug
// K_MSGQ_DEFINE(msqLedandKey, sizeof(struct LcdMsg), 8, 4);

// int messageQueueInit()
// {
//     msqReceivedFromMQTT = mq_open("/msqReceivedFromMQTT", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
//     msqSendToMQTT = mq_open("/msqSendToMQTT", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
//     msqRfid730 = mq_open("/msqRfid730", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
//     msqRfidRc = mq_open("/msqRfidRc", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
//     msqLcd = mq_open("/msqLcd", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL);
// }
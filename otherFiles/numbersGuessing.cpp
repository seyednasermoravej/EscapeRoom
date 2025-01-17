#include "numbersGuessing.h"


LOG_MODULE_REGISTER(NumbersGuessing, LOG_LEVEL_INF);


NumbersGuessing:: NumbersGuessing()
{
    
    ledandkeysThreadCreate();

}

void NumbersGuessing:: sendButtonsVal()
{
    // int dev_address = 0;
    // char dev_addr_char[5];
    // dev_address=addrKeysVal();
    // sprintf(dev_addr_char,"%d",dev_address);
    // LOG_INF("The Device Address is %d\n\r",dev_address);
    // struct MqttMsg send = {0};
    // strcpy(send.topic, DEV_ADDR_TOPIC);
    // strcpy(send.msg, dev_addr_char);
    // k_msgq_put(&msqSendToMQTT, &send, K_NO_WAIT);
    LOG_INF("The Device Address is \n\r");
}

void NumbersGuessing:: segementIncrement()
{
    // int dev_address = 0;
    // char dev_addr_char[5];
    // dev_address=addrKeysVal();
    // sprintf(dev_addr_char,"%d",dev_address);
    LOG_INF("The Device Address is \n\r");
    // struct MqttMsg send = {0};
    // strcpy(send.topic, DEV_ADDR_TOPIC);
    // strcpy(send.msg, dev_addr_char);
    // k_msgq_put(&msqSendToMQTT, &send, K_NO_WAIT);
}

void NumbersGuessing:: messageHandler(struct MqttMsg *msg)
{

        k_msgq_put(&msqLedandKey, msg, K_NO_WAIT);
}
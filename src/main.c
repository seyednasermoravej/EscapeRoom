#include "main.h"
#include <stdlib.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);



int threadsCreation();



int main()
{ 
    //performing new update on firmware

    // messageQueueInit();
    dhcpClient();
    //http request for getting DFU
    sem_wait(&dhcpActive);
    sem_destroy(&dhcpActive); 
    threadsCreation();
    char *send = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
    memset(send, 0, MESSAGE_QUEUE_LEN);
    strcpy(send, "allah");
    char buf[4];
    int counter = 0;
    while(1)
    {
        strcat(send, itoa(counter, buf, 10));
        counter++;
        int ret = k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
        k_msleep(2000);
    }
    // return 0;
}


int threadsCreation()
{
    mqttThreadCreate();
}
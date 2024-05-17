#include "main.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);



int threadsCreation();



int main()
{ 
    //performing new update on firmware

    messageQueueInit();
    dhcpClient();
    //http request for getting DFU
    sem_wait(&dhcpActive);
    sem_destroy(&dhcpActive); 
    threadsCreation();
    while(1)
    {
        mqttConnection();
        k_msleep(1000);

    }
    return 0;
}


int threadsCreation()
{

}
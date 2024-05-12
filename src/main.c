#include "main.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);



int threadsCreation();



int main()
{ 
    //performing new update on firmware

    messageQueueInit();
    threadsCreation();
    dhcpClient();
    //http request for getting DFU
    while(!success);
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
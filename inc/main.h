#ifndef __MAIN__H__
#define __MAIN__H__


#include "dhcpClient.h"
#include "mqttConnection.h"
#include "zephyr/drivers/gpio.h"
#include "messageQueues.h"
#include <zephyr/drivers/i2c.h>

#define ADDR_NODE	DT_ALIAS(addr)
int dnsResolver(char *queryName, char *serverIpAddress);
extern sem_t dhcpActive;
#endif


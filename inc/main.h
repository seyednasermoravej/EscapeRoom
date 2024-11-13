#ifndef __MAIN__H__
#define __MAIN__H__

#include <zephyr/drivers/i2c.h>
#include <stdlib.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/drivers/hwinfo.h>

#include "dnsResolver.h"
#include "mqttConnection.h"
#include "zephyr/drivers/gpio.h"
#include "messageQueues.h"


#define ADDR_NODE	DT_ALIAS(addr)
#endif


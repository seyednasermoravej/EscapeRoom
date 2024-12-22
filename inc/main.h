#ifndef __MAIN__H__
#define __MAIN__H__

#include <zephyr/drivers/i2c.h>
#include <stdlib.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/drivers/hwinfo.h>

#include "dnsResolver.h"
#include "zephyr/drivers/gpio.h"
#include "messageQueues.h"
#include "dhcpClient.h"
#include <zephyr/usb/usbd.h>

#include <zephyr/usb/usb_device.h>
#include <sample_usbd.h>
extern char deviceId[17]; 

#define NASER 
#define ADDR_NODE	DT_ALIAS(addr)
#endif


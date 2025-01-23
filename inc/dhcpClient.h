#ifndef __DHCP_H__
#define __DHCP_H__

#include <zephyr/logging/log.h>


#include <zephyr/kernel.h>
#include <zephyr/linker/sections.h>
#include <errno.h>
#include <stdio.h>

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <semaphore.h>
#include "main.h"

#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>

extern uint8_t devId[8];
#define WIZNET_OUI_B0	0x00
#define WIZNET_OUI_B1	0x08
#define WIZNET_OUI_B2	0xdc

#ifdef __cplusplus
extern "C" {
#endif
#define DHCP_OPTION_NTP (42)
extern sem_t dhcpActive;
int dhcpClient(const char *deviceName);

#ifdef __cplusplus
}
#endif
#endif
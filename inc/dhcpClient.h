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

#ifdef __cplusplus
extern "C" {
#endif
#define DHCP_OPTION_NTP (42)
extern sem_t dhcpActive;
int dhcpClient();

#ifdef __cplusplus
}
#endif
#endif
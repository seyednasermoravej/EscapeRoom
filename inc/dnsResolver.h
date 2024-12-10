#ifndef __DNS_RESOLVER__H__
#define __DNS_RESOLVER__H__


#include <zephyr/kernel.h>
#include <zephyr/linker/sections.h>
#include <errno.h>
#include <stdio.h>

#include <zephyr/net/net_core.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/dns_resolve.h>

#include "net_sample_common.h"

#include <zephyr/logging/log.h>

#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#ifdef __cplusplus
extern "C" {
#endif
int dnsResolver(const char *deviceName, char *queryName, char *serverIpAddress);
extern sem_t dnsActive;

#ifdef __cplusplus
}
#endif
#endif
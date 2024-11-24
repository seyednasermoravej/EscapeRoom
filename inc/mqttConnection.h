#ifndef __MQTT__H__
#define __MQTT__H__

#include <zephyr/logging/log.h>

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>

#include <string.h>
#include <errno.h>

#include "config.h"

#include "messageQueues.h"

#ifdef __cplusplus
extern "C" {
#endif
extern bool command;

#define MQTT_STACK_SIZE     1024 * 8 
#define MQTT_PRIORITY      8 
// #define SERVER_IP_ADDRESS    "192.168.100.169"
#define SERVER_IP_ADDRESS    "5.196.78.28"
// #define SERVER_IP_ADDRESS    "91.121.93.94"
extern void mqttThreadCreate(char *);

extern void mqttEntryPoint(void *, void *, void *);

#ifdef __cplusplus
}
#endif
#endif
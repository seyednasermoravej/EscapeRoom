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

#define MQTT_STACK_SIZE     1024 * 4 
#define MQTT_PRIORITY      8 
extern void mqttThreadCreate();

extern void mqttEntryPoint(void *, void *, void *);

#ifdef __cplusplus
}
#endif
#endif
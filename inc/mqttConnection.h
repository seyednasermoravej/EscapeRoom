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

int mqttConnection();

#ifdef __cplusplus
}
#endif
#endif
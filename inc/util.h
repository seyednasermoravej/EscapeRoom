#ifndef __UTIL_H__
#define __UTIL__H__

#include <zephyr/logging/log.h>

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>

#include <string.h>
#include <errno.h>

#include <zephyr/sys/util.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hex2stringMac(const uint8_t *hex, int length, char *buf);


#ifdef __cplusplus
}
#endif
#endif
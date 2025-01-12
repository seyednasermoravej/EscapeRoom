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




#include <zephyr/drivers/watchdog.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <zephyr/sys/sys_heap.h>
#define BRAM 
// #define WATCH_DOG
/*
 * To use this sample the devicetree's /aliases must have a 'watchdog0' property.
 */
#if DT_HAS_COMPAT_STATUS_OKAY(st_stm32_window_watchdog)
#define WDT_MAX_WINDOW  100U
#elif DT_HAS_COMPAT_STATUS_OKAY(nordic_nrf_wdt)
/* Nordic supports a callback, but it has 61.2 us to complete before
 * the reset occurs, which is too short for this sample to do anything
 * useful.  Explicitly disallow use of the callback.
 */
#define WDT_ALLOW_CALLBACK 0
#elif DT_HAS_COMPAT_STATUS_OKAY(raspberrypi_pico_watchdog)
#define WDT_ALLOW_CALLBACK 0
#elif DT_HAS_COMPAT_STATUS_OKAY(gd_gd32_wwdgt)
#define WDT_MAX_WINDOW 24U
#define WDT_MIN_WINDOW 18U
#define WDG_FEED_INTERVAL 12U
#elif DT_HAS_COMPAT_STATUS_OKAY(intel_tco_wdt)
#define WDT_ALLOW_CALLBACK 0
#define WDT_MAX_WINDOW 3000U
#elif DT_HAS_COMPAT_STATUS_OKAY(nxp_fs26_wdog)
#define WDT_MAX_WINDOW  1024U
#define WDT_MIN_WINDOW	320U
#define WDT_OPT 0
#define WDG_FEED_INTERVAL (WDT_MIN_WINDOW + ((WDT_MAX_WINDOW - WDT_MIN_WINDOW) / 4))
#endif

#ifndef WDT_ALLOW_CALLBACK
#define WDT_ALLOW_CALLBACK 1
#endif

#ifndef WDT_MAX_WINDOW
#define WDT_MAX_WINDOW  8000U
#endif

#ifndef WDT_MIN_WINDOW
#define WDT_MIN_WINDOW  0U
#endif

#ifndef WDG_FEED_INTERVAL
#define WDG_FEED_INTERVAL 8000U
#endif

#ifndef WDT_OPT
#define WDT_OPT WDT_OPT_PAUSE_HALTED_BY_DBG
#endif







extern int wdt_channel_id;
extern const struct device *const wdt;






extern char deviceId[17]; 

#define ADDR_NODE	DT_ALIAS(addr)
#endif


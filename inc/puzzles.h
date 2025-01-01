#ifndef __PUZZLES__H__
#define __PUZZLES__H__


// #ifdef __cplusplus
// extern "C" {
// #endif
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>

#include "dnsResolver.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define NVS_PARTITION		storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)


#include "main.h"
#include "gate.h"
#include "servos.h"
#include "console.h"
#include "fridge.h"
#include "numbersGuessing.h"
#include "laboratory.h"
#include "platform.h"
#include "cabinet.h"
#include "door.h"
#include "xray.h"
#include "doorKeypad.h"
#include "ventilator.h"
#include "heartMonitor.h"
#include "blinds.h"
#include "powerPanel.h"
#include "scale.h"
#include "colorTubes.h"
#include "entranceDoor.h"
#include "exitDoor.h"
#include "pneumaPost.h"
#include "drawers.h"
#include "heartBox.h"
#include "heart.h"

#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#else
#define BUILT_IN_NODE	DT_ALIAS(built_in_led)
#if !DT_NODE_HAS_STATUS(BUILT_IN_NODE, okay)
#error "Unsupported board: BUILT_IN devicetree alias is not defined"
#endif
#endif

#define PUZZLE_STACK_SIZE                  4096
#define PUZZLE_PRIORITY                    9 
#define PUZZLE_TYPE_NAME_MAX_LEN           128

class Puzzles
{
public:
    Puzzles(struct nvs_fs *fs);
    void messageHandler(struct MqttMsg *msg);
    bool deviceSpecified = false;
    void puzzleTypeSelection(char *type);
    int  writeDeviceName(char *name);
    Puzzle *puzzle;
    char name[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    int enableWatchDog();

private:

    struct nvs_fs *fs;

    int builtIntLedInit();
    int nvsInit();
    void readInfosFromMemory();



};
#endif
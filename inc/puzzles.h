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

// #include "dnsResolver.h"

#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define NVS_PARTITION		storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)



#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/logging/log.h>
#include <zephyr/fs/fs.h>

#if defined(CONFIG_FAT_FILESYSTEM_ELM)

#include <ff.h>

/*
 *  Note the fatfs library is able to mount only strings inside _VOLUME_STRS
 *  in ffconf.h
 */
#if defined(CONFIG_DISK_DRIVER_MMC)
#define DISK_DRIVE_NAME "SD2"
#else
#define DISK_DRIVE_NAME "SD"
#endif

// #define DISK_MOUNT_PT "/SD:"
#define DISK_MOUNT_PT "/"DISK_DRIVE_NAME":"

#endif

#if defined(CONFIG_FAT_FILESYSTEM_ELM)
#define FS_RET_OK FR_OK
#else
#define FS_RET_OK 0
#endif


#include "main.h"
#include "puzzle.h"
#include "console.h"
#include "fridge.h"
#include "platform.h"
#include "cabinet.h"
#include "door.h"
#include "xray.h"
#include "doorKeypad.h"
#include "ventilator.h"
#include "heartMonitor.h"
#include "blinds.h"
#include "powerPanel.h"
// #include "scale.h"
#include "colorTubes.h"
#include "sticks.h"
#include "entranceDoor.h"
#include "exitDoor.h"
#include "pneumaPost.h"
#include "defib.h"
#include "drawers.h"
#include "heartBox.h"
#include "heart.h"


#include "ota.h"

#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#elif defined(CONFIG_BOARD_ESP32S3_DEVKITC)
#else
#define BUILT_IN_NODE	DT_ALIAS(built_in_led)
#if !DT_NODE_HAS_STATUS(BUILT_IN_NODE, okay)
#error "Unsupported board: BUILT_IN devicetree alias is not defined"
#endif
#endif

#define PUZZLE_STACK_SIZE                  2 * 8192
// #define PUZZLE_STACK_SIZE                  4096
#define PUZZLE_PRIORITY                    9
#define PUZZLE_TYPE_NAME_MAX_LEN           128

void play_wav(const char *path);
class Puzzles
{
public:
    Puzzles(struct nvs_fs *fs);
    void messageHandler(struct MqttMsg *msg);
    bool deviceSpecified = false;
    void puzzleTypeSelection(char *type);
    int  writeDeviceName(char *name);
    Puzzle *puzzle;
#if(CONFIG_BOOTLOADER_MCUBOOT)
    Ota *ota;
#endif
    char name[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    int enableWatchDog();
    void eraseStorage();

private:

    struct nvs_fs *fs;

    int builtIntLedInit();
    int nvsInit();
    void readInfosFromMemory();



};
#endif

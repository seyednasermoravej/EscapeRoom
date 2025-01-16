#include "fridge.h"
#include "lvgl_mem.h"
LOG_MODULE_REGISTER(fridge, LOG_LEVEL_DBG);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(fridge_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

#define STRIP_NODE		DT_NODELABEL(ws2812)

#if DT_NODE_HAS_PROP(DT_NODELABEL(ws2812), chain_length)
static const uint8_t wsChainLength = DT_PROP(DT_NODELABEL(ws2812), chain_length);
#else
#error Unable to determine length of LED strip
#endif

static const struct device *const pio1_dev = DEVICE_DT_GET(DT_NODELABEL(pio1));



#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/sys_heap.h>

#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include <ff.h>


#define DISK_DRIVE_NAME "SD"
#define DISK_MOUNT_PT "/"DISK_DRIVE_NAME":"
#define FS_RET_OK FR_OK

static FATFS fat_fs;

/* mounting info */
static struct fs_mount_t mp = {
	.type = FS_FATFS,
	.fs_data = &fat_fs,
};


static const char *disk_mount_pt = DISK_MOUNT_PT;



#define DISPLAY_NODE          DT_CHOSEN(zephyr_display)
#define DISPLAY_WIDTH  DT_PROP(DISPLAY_NODE, width)
#define DISPLAY_HEIGHT DT_PROP(DISPLAY_NODE, height)


static lv_display_t *display;
static struct lvgl_disp_data disp_data = {0};

static int lvgl_allocate_rendering_buffers(lv_display_t *display)
{
	void *buf0 = NULL;
	void *buf1 = NULL;
	uint16_t buf_nbr_pixels;
	uint32_t buf_size;
	struct lvgl_disp_data *data = (struct lvgl_disp_data *)lv_display_get_user_data(display);
	uint16_t hor_res = lv_display_get_horizontal_resolution(display);
	uint16_t ver_res = lv_display_get_vertical_resolution(display);

	buf_nbr_pixels = (CONFIG_LV_Z_VDB_SIZE * hor_res * ver_res) / 100;
	/* one horizontal line is the minimum buffer requirement for lvgl */
	if (buf_nbr_pixels < hor_res) {
		buf_nbr_pixels = hor_res;
	}

	switch (data->cap.current_pixel_format) {
	case PIXEL_FORMAT_ARGB_8888:
		buf_size = 4 * buf_nbr_pixels;
		break;
	case PIXEL_FORMAT_RGB_888:
		buf_size = 3 * buf_nbr_pixels;
		break;
	case PIXEL_FORMAT_RGB_565:
		buf_size = 2 * buf_nbr_pixels;
		break;
	case PIXEL_FORMAT_MONO01:
	case PIXEL_FORMAT_MONO10:
		buf_size = buf_nbr_pixels / 8 + 8;
		buf_size += (buf_nbr_pixels % 8) == 0 ? 0 : 1;
		break;
	default:
		return -ENOTSUP;
	}

	buf0 = lv_malloc(buf_size);
	if (buf0 == NULL) {
		LOG_ERR("Failed to allocate memory for rendering buffer");
		return -ENOMEM;
	}


	buf1 = lv_malloc(buf_size);
	if (buf1 == NULL) {
		lv_free(buf0);
		LOG_ERR("Failed to allocate memory for rendering buffer");
		return -ENOMEM;
	}


	lv_display_set_buffers(display, buf0, buf1, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
	return 0;
}





// void check_lvgl_memory(void)
// {
//     lv_mem_monitor_t mem_mon;
//     lv_mem_monitor(&mem_mon);

//     LOG_DBG("LVGL Memory Statistics:\n");
//     LOG_DBG("  Total memory:   %zu bytes\n", mem_mon.total_size);
//     // printk("  Used memory:    %zu bytes\n", mem_mon.used_size);
//     LOG_DBG("  Free memory:    %zu bytes\n", mem_mon.free_size);
//     LOG_DBG("  Largest free block: %zu bytes\n", mem_mon.free_biggest_size);
//     // printk("  Memory fragmentation: %u%%\n", mem_mon.frag_percent);
// }
extern int lvgl_init();
static uint32_t count;
Fridge:: Fridge(const char *room, const char *type): Puzzle(room, type)
{
    device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(display_mipi_dbi)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(ili9488_buydisplay_3_5_tft_touch_arduino)));
    

    int err = 0;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		// return 0;
	}
    lvgl_init();


    device_init(DEVICE_DT_GET(DT_NODELABEL(sdhc0)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(mmc)));




	mp.mnt_point = disk_mount_pt;
	int res = fs_mount(&mp);
	if (res == FS_RET_OK) {
		printk("Disk mounted.\n");
		}
	else {
		printk("Error mounting disk.\n");
	}











    device_init(pio1_dev);
    ledStrip = new LedStrip(DEVICE_DT_GET(STRIP_NODE), wsChainLength);

    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(allRelays); i++){
        if (!device_is_ready(allRelays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&allRelays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    keypad = new Keypad(mqttCommand);


    creatingMqttList(11);
}

void Fridge:: creatingMqttList(uint16_t _mqttCount)
{
    uint8_t numOfDisplays = 1;

    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }
    for(uint8_t i = 0; i < wsChainLength; i++) /// chain length in overlay
    {
        sprintf(topic, "%sws2811_%d", mqttCommand, i + 1);
        mqttList[i + ARRAY_SIZE(allRelays)] = *createMqttTopic(topic);
    }

    // for(uint8_t i = 0; i < numOfDisplays; i++)
    // {
    //     sprintf(topic, "%sdisplay%d", mqttCommand, i + 1);
    //     mqttList[wsChainLength + ARRAY_SIZE(allRelays) + i] = *createMqttTopic(topic);
    // }
    sprintf(topic, "%sdisplay", mqttCommand);
    mqttList[wsChainLength + ARRAY_SIZE(allRelays)] = *createMqttTopic(topic);

    mqttCount = wsChainLength + ARRAY_SIZE(allRelays) + numOfDisplays;
}


void Fridge:: messageHandler(struct MqttMsg *msg)
{
    int rc;
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strcmp(command, "display") == 0)
        {

            LV_IMAGE_DECLARE(bram);
            lv_obj_t * my_screen = lv_image_create(lv_scr_act());
            if(strcmp(msg->msg, "sd bram") == 0)
            {
                lv_image_set_src(my_screen, DISK_MOUNT_PT"/bram.bin");
                LOG_INF("from sd card");
            }
            else if(strcmp(msg->msg, "internal bram") == 0)
            {
                lv_image_set_src(my_screen, &bram);
                LOG_INF("from internal memory");
            }
            else if(strcmp(msg->msg, "sd Bram") == 0)
            {
                lv_image_set_src(my_screen, DISK_MOUNT_PT"/bram.bin");
                LOG_INF("from sd card");
            }
            else if(strcmp(msg->msg, "lion") == 0)
            {
                lv_image_set_src(my_screen, DISK_MOUNT_PT"/lion.bin");
                LOG_INF("from sd card");
            }
            else if(strcmp(msg->msg, "rainbow") == 0)
            {
                lv_image_set_src(my_screen, DISK_MOUNT_PT"/rainbow.bin");
                LOG_INF("from sd card");
            }
            else if(strcmp(msg->msg, "raspbbery") == 0)
            {
                lv_image_set_src(my_screen, DISK_MOUNT_PT"/raspbbery.bin");
                LOG_INF("from sd card");
            }
            lv_obj_align(my_screen, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_size(my_screen,320,480);

            lv_task_handler();
            display_blanking_off(display_dev);

            // while (1) {
                lv_task_handler();
                // k_sleep(K_MSEC(10));
            // }
        }
        else if(strstr(command, "relay") != NULL)
        {
            char field[] = "relay";
            int commandIdx = peripheralIdx(field, command);
            uint8_t relayIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
            {
                if(relayIdx == 1)
                {
                    relayOperation(msg->msg, &allRelays[relayIdx], true);
                }
                else
                {
                    relayOperation(msg->msg, &allRelays[0], false);
                }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else if(strstr(command, "ws2811_") != NULL)
        {
            char field[] = "ws2811_";
            int commandIdx = peripheralIdx(field, command);
            uint8_t ws2811Idx = commandIdx - 1;
            if((commandIdx > 0 ) && (ws2811Idx < 8))
            {
                 //LOG_ERR("a valid index");
                struct led_rgb color_leds = retrieveColors(msg->msg);
                LOG_INF("r: %u g: %u b: %u", color_leds.r, color_leds.g, color_leds.b);

                rc = ledStrip->update(color_leds, ws2811Idx);
                if (rc) {
				    LOG_ERR("couldn't update strip: %d", rc);
			    }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }
        }
    }
    else
        LOG_INF("the command is not valid");
}


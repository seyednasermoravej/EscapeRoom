#include "fridge.h"

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

static uint32_t count;
Fridge:: Fridge(const char *room, const char *type): Puzzle(room, type)
{
    const struct device *tftLcd = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(tftLcd)) {
		LOG_ERR("Device not ready, aborting test");
		// return 0;
	}
    // printk("LVGL custom allocator: %d\n", CONFIG_LV_MEM_CUSTOM);
    // lv_init();
    LOG_DBG("LVGL memory pool size: %d\n", CONFIG_LV_Z_MEM_POOL_SIZE); 
    // while(1)
    // {
    // check_lvgl_memory();
    // k_msleep(1000);
    LV_IMG_DECLARE(bram);
    // check_lvgl_memory();

    // }
	lv_obj_t * my_bram = lv_img_create(lv_scr_act());
	lv_img_set_src(my_bram, &bram);
	lv_obj_align(my_bram, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(my_bram,320,480);
    // /////////////////
	lv_task_handler();
	display_blanking_off(tftLcd);

	while (1) {
		lv_task_handler();
		k_sleep(K_MSEC(10));
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
    creatingMqttList(11);
    keypad = new Keypad(mqttCommand);

    // device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    // device_init(DEVICE_DT_GET(DT_NODELABEL(display_mipi_dbi)));
    // device_init(DEVICE_DT_GET(DT_NODELABEL(ili9488_buydisplay_3_5_tft_touch_arduino)));



    // 	char count_str[11] = {0};
	// const struct device *display_dev;
	// lv_obj_t *hello_world_label;
	// lv_obj_t *count_label;

    //device_init(tftLcd);
	//display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	// 	hello_world_label = lv_label_create(lv_scr_act());

	// lv_label_set_text(hello_world_label, "Hello world!");
	// lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	// count_label = lv_label_create(lv_scr_act());
	// lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	// lv_task_handler();
	// display_blanking_off(display_dev);

	// while (1) {
	// 	if ((count % 100) == 0U) {
	// 		sprintf(count_str, "%d", count/100U);
	// 		lv_label_set_text(count_label, count_str);
	// 	}
	// 	lv_task_handler();
	// 	++count;
	// 	k_sleep(K_MSEC(10));
	// }
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
            ////display logic
            LOG_ERR("display logic");
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


#include "fridge.h"

LOG_MODULE_REGISTER(fridge, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(fridge_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

#define STRIP_NODE		DT_NODELABEL(ws2812)

#if DT_NODE_HAS_PROP(DT_NODELABEL(ws2812), chain_length)
#define STRIP_NUM_PIXELS	DT_PROP(DT_NODELABEL(ws2812), chain_length)
#else
#error Unable to determine length of LED strip
#endif

// static const struct device *const pio1_dev = DEVICE_DT_GET(DT_NODELABEL(pio1));


static uint32_t count;
static void lv_btn_click_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	count = 0;
}
const struct device *tftLcd = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
Fridge:: Fridge(const char *room, const char *type): Puzzle(room, type)
{

    // device_init(pio1_dev);
    // if (!device_is_ready(pio1_dev)) {
	// 	LOG_ERR("strip Device not ready, aborting test");
	// } 


    static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);  
    // device_init(strip); 
	if (!device_is_ready(strip)) {
		LOG_ERR("strip Device not ready, aborting test");
	}    
    ledStrip = new LedStrip(strip, STRIP_NUM_PIXELS);

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
    //char topic[64];
    //sprintf(topic, "%s/%s/", roomName, puzzleTypeName);
    keypad = new Keypad(mqttCommand);
   


    



    // 	char count_str[11] = {0};
	// const struct device *display_dev;
	// lv_obj_t *hello_world_label;
	// lv_obj_t *count_label;

    //device_init(tftLcd);
	//display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	// if (!device_is_ready(display_dev)) {
	// 	LOG_ERR("Device not ready, aborting test");
	// 	// return 0;
	// }
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

    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }

    sprintf(topic, "%sdisplay", mqttCommand);
    mqttList[2] = *createMqttTopic(topic);

    for(uint8_t i = 0; i < 8; i++) /// chain length in overlay
    {
        sprintf(topic, "%sws2811_%d", mqttCommand, i + 1);
        mqttList[i+3] = *createMqttTopic(topic);
    }

    mqttCount = _mqttCount;

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


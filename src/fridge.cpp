#include "fridge.h"

LOG_MODULE_REGISTER(fridge, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(fridge_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


static uint32_t count;
static void lv_btn_click_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	count = 0;
}
const struct device *tftLcd = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
Fridge:: Fridge(const char *room, const char *type): Puzzle(room, type)
{
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
    creatingMqttList(3);
    char topic[64];
    sprintf(topic, "%s/%s/", roomName, puzzleTypeName);
    keypad = new Keypad(topic);



    	char count_str[11] = {0};
	const struct device *display_dev;
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;

    device_init(tftLcd);
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		// return 0;
	}
		hello_world_label = lv_label_create(lv_scr_act());

	lv_label_set_text(hello_world_label, "Hello world!");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);

	while (1) {
		if ((count % 100) == 0U) {
			sprintf(count_str, "%d", count/100U);
			lv_label_set_text(count_label, count_str);
		}
		lv_task_handler();
		++count;
		k_sleep(K_MSEC(10));
	}
}

void Fridge:: creatingMqttList(uint16_t _mqttCount)
{

    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%s/%s/relay%d", roomName, puzzleTypeName, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }

    sprintf(topic, "%s/%s/display", roomName, puzzleTypeName);
    mqttList[2] = *createMqttTopic(topic);
    mqttCount = _mqttCount;

}


void Fridge:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strcmp(command, "display") == 0)
        {
            ////display logic
        }
        else if(strcmp(command, "relay") == 0)
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
    }
    else
        LOG_INF("the command is not valid");
}
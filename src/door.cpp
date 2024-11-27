#include "door.h"

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(door_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


Door:: Door(const char *room, const char *type): Puzzle(room, type)
{
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(relays); i++){
        if (!device_is_ready(relays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&relays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    creatingMqttList();
}

void Door:: creatingMqttList()
{

	mqttList[0] = codeRed_door_relay1_topic;
	mqttList[1] = codeRed_door_relay2_topic;
    mqttCount = 2;

}
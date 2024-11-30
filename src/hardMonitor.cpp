#include "hartMonitor.h"

LOG_MODULE_REGISTER(hartMonitor, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

static const struct gpio_dt_spec multiplexer[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(ventilator_analog_multiplexer), gpios, DT_SPEC_AND_COMMA_GATE)
};
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),


/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};





HartMonitor:: HartMonitor(const char *room, const char *type): Puzzle(room, type)
{
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(multiplexer); i++){
        if (!device_is_ready(multiplexer[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&multiplexer[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    adcs = new Adcs(adc_channels, 1);
    // creatingMqttList(4);
}

void HartMonitor:: creatingMqttList(uint16_t _mqttCount)
{

	// mqttList[0] = codeRed_ventilator_relay1_topic;
	// mqttList[1] = codeRed_ventilator_relay2_topic;
	// mqttList[2] = codeRed_ventilator_relay3_topic;
	// mqttList[3] = codeRed_ventilator_relay4_topic;
    // mqttCount = _mqttCount;

}


void HartMonitor:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    // if(strcmp(msg->topic, CODE_RED_VENTILATOR_RELAY1_TOPIC) == 0)
    // {
    //     if(strcmp(msg->msg, "on") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[0], 1);
    //     }
    //     else if(strcmp(msg->msg, "off") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[0], 0);
    //     }
    //     else
    //     {
    //         LOG_INF("The command is not valid");
    //     }
    // }
    // else if(strcmp(msg->topic, CODE_RED_VENTILATOR_RELAY2_TOPIC) == 0)
    // {
    //     if(strcmp(msg->msg, "on") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[1], 1);
    //     }
    //     else if(strcmp(msg->msg, "off") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[1], 0);
    //     }
    //     else
    //     {
    //         LOG_INF("The command is not valid");
    //     }
    // } 
    // else if(strcmp(msg->topic, CODE_RED_VENTILATOR_RELAY3_TOPIC) == 0)
    // {
    //     if(strcmp(msg->msg, "on") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[2], 1);
    //     }
    //     else if(strcmp(msg->msg, "off") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[2], 0);
    //     }
    //     else
    //     {
    //         LOG_INF("The command is not valid");
    //     }
    // } 
    // else if(strcmp(msg->topic, CODE_RED_VENTILATOR_RELAY4_TOPIC) == 0)
    // {
    //     if(strcmp(msg->msg, "on") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[3], 1);
    //     }
    //     else if(strcmp(msg->msg, "off") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[3], 0);
    //     }
    //     else
    //     {
    //         LOG_INF("The command is not valid");
    //     }
    // } 
    // else
    //     LOG_INF("the command is not valid");
}


uint16_t HartMonitor:: readAdc(uint8_t channel)
{
    for(uint32_t i = 0; i < 3; i++)
    {
        gpio_pin_set_dt(&multiplexer[i], channel & BIT(i));
    }
    MqttMsg msg = {0};
    sprintf(msg.topic, "%s/%s/%u", roomName, puzzleTypeName, channel);
    sprintf(msg.msg, "%u", adcs->readAdc(0));
}



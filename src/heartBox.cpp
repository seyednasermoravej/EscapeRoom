#include "heartBox.h"

LOG_MODULE_REGISTER(heartBox, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_box_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


HeartBox:: HeartBox(const char *room, const char *type): Puzzle(room, type)
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
    creatingMqttList(11);
    char topic[64];
    sprintf(topic, "%s/%s/", roomName, puzzleTypeName);
    keypad = new Keypad(topic);
    // ledStrip = new LedStrip()
}

void HeartBox:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_heartBox_display1_topic;
	mqttList[1] = codeRed_heartBox_display2_topic;
	mqttList[2] = codeRed_heartBox_relay1_topic;
	mqttList[3] = codeRed_heartBox_ws2811a_topic;
	mqttList[4] = codeRed_heartBox_ws2811b_topic;
	mqttList[5] = codeRed_heartBox_ws2811c_topic;
	mqttList[6] = codeRed_heartBox_ws2811d_topic;
	mqttList[7] = codeRed_heartBox_ws2811e_topic;
	mqttList[8] = codeRed_heartBox_ws2811f_topic;
	mqttList[9] = codeRed_heartBox_ws2811g_topic;
	mqttList[10] = codeRed_heartBox_ws2811h_topic;
    mqttCount = _mqttCount;

}


void HeartBox:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_HEART_BOX_DISPLAY1_TOPIC) == 0)
    {
        //??????????????????logic is unknown
        // if(strcmp(msg->msg, "on") == 0)
        // {
        //     gpio_pin_set_dt(&relays[0], 1);
        // }
        // else if(strcmp(msg->msg, "off") == 0)
        // {
        //     gpio_pin_set_dt(&relays[0], 0);
        // }
        // else
        // {
        //     LOG_INF("The command is not valid");
        // }
        //??????????????????logic is unknown
    }
    else if(strcmp(msg->topic, CODE_RED_HEART_BOX_DISPLAY2_TOPIC) == 0)
    {
        //??????????????????logic is unknown
        // if(strcmp(msg->msg, "on") == 0)
        // {
        //     gpio_pin_set_dt(&relays[0], 1);
        // }
        // else if(strcmp(msg->msg, "off") == 0)
        // {
        //     gpio_pin_set_dt(&relays[0], 0);
        // }
        // else
        // {
        //     LOG_INF("The command is not valid");
        // }
        //??????????????????logic is unknown
    }
    else if(strcmp(msg->topic, CODE_RED_HEART_BOX_RELAY1_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[0], 1);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[0], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_HEART_BOX_WS2811A_TOPIC) == 0)
    {

        // char rStr[4];

    } 
    else
        LOG_INF("the command is not valid");
}

struct led_rgb HeartBox:: retrieveColors(char *str)
{
    struct led_rgb rgb;
    char buf[4];
    uint8_t i = 0;
    while(str[i] != 'G')
    {
        buf[i - 1] = str[i];
        i++;
    }
    buf[i] = '\0';
    rgb.r = atoi(buf);
    memset(buf, 0, 4);
    while(str[i] != 'B')
    {
        buf[i - 1] = str[i];
        i++;
    }
    buf[i] = '\0';
    rgb.g = atoi(buf);
    memset(buf, 0, 4);
    while(str[i] != strlen(str))
    {
        buf[i - 1] = str[i];
        i++;
    }
    buf[i] = '\0';
    rgb.b = atoi(buf);
    return rgb;

}
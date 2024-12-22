#include "doorKeypad.h"

LOG_MODULE_REGISTER(doorKeypad, LOG_LEVEL_INF);
static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(door_keypad_buttons));

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

const struct gpio_dt_spec display[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(door_keypad_display), gpios, DT_SPEC_AND_COMMA_GATE)
};
static DoorKeypad *instance = nullptr;

void DoorKeypad:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}


void DoorKeypad:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};

        if(val->value)
        {
            sprintf(msg.topic, "%s/%s/button%d", roomName, puzzleTypeName, (val->code - INPUT_BTN_0) + 1);
            sprintf(msg.msg, "true");
            LOG_INF("%s, %s", msg.topic, msg.msg);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
      
    }

}

const uint8_t digit_segments[] = {
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};
// void displayDigits()
// {

// }
DoorKeypad:: DoorKeypad(const char * room, const char *type): Puzzle(room, type)
{
    LOG_INF("%s/%s", room, type);
    LOG_INF("%s/%s", roomName, puzzleTypeName);
    creatingMqttList(1);
    device_init(buttons);
    instance = this;
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void*)this);
    device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(sevensegment)));

    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(display); i++){
        if (!device_is_ready(display[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&display[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }

	while(1)
	{
        gpio_pin_set_dt(&display[0], 1);
        for(uint8_t i = 0; i < 8; i++)
        {
            gpio_pin_set_dt(&display[8 + i], (digit_segments[0]>> i) &0x01);
        }
        LOG_INF("Entered while loop");
        gpio_pin_set_dt(&display[1], 1);
        gpio_pin_set_dt(&display[2], 1);
        gpio_pin_set_dt(&display[3], 1);
        gpio_pin_set_dt(&display[4], 1);
        gpio_pin_set_dt(&display[5], 1);
        gpio_pin_set_dt(&display[6], 1);
        gpio_pin_set_dt(&display[7], 1);
        // gpio_pin_set_dt(&display[9], 1);
        // gpio_pin_set_dt(&display[10], 1);
        // gpio_pin_set_dt(&display[11], 1);
        // gpio_pin_set_dt(&display[12], 1);
        // gpio_pin_set_dt(&display[13], 1);
        // gpio_pin_set_dt(&display[14], 1);
        // gpio_pin_set_dt(&display[15], 1);
        k_msleep(100);
        // gpio_pin_set_dt(&display[0], 0);
        // gpio_pin_set_dt(&display[1], 0);
        // gpio_pin_set_dt(&display[2], 0);
        // gpio_pin_set_dt(&display[3], 0);
        // gpio_pin_set_dt(&display[4], 0);
        // gpio_pin_set_dt(&display[5], 0);
        // gpio_pin_set_dt(&display[6], 0);
        // gpio_pin_set_dt(&display[7], 0);
        // gpio_pin_set_dt(&display[8], 0);
        // gpio_pin_set_dt(&display[9], 0);
        // gpio_pin_set_dt(&display[10], 0);
        // gpio_pin_set_dt(&display[11], 0);
        // gpio_pin_set_dt(&display[12], 0);
        // gpio_pin_set_dt(&display[13], 0);
        // gpio_pin_set_dt(&display[14], 0);
        // gpio_pin_set_dt(&display[15], 0);
        // k_usleep(1);
    }	



}

void DoorKeypad:: creatingMqttList(uint16_t _mqttCount)
{
    char topic[128] = {0};
    sprintf(topic, "%s/%s/display", roomName, puzzleTypeName);
    mqttList[0] = *createMqttTopic(topic);
    mqttCount = _mqttCount;
}
void DoorKeypad:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        char field[] = "display";
        if((strcmp(field, command) == 0))
        {
            // display->print(msg->msg);
        }
        else
        {
            LOG_ERR("Not a valid index");
        }
    }
}
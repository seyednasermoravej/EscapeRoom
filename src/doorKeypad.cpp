#include "doorKeypad.h"

LOG_MODULE_REGISTER(doorKeypad, LOG_LEVEL_INF);
static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(door_keypad_buttons));

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec digits[] = {
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
DoorKeypad:: DoorKeypad(const char * room, const char *type): Puzzle(room, type)
{
    LOG_INF("%s/%s", room, type);
    LOG_INF("%s/%s", roomName, puzzleTypeName);
    creatingMqttList(1);
    device_init(buttons);
    instance = this;
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void*)this);
	display = new TM74HC595LedTube (digits[0].port, digits[0].pin, digits[1].port, digits[1].pin, digits[2].port, digits[2].pin, true,  8);
    display->begin();
	// display->print("98716236");
	// while(1)
	// {}	



}

void DoorKeypad:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_doorKeypad_display_topic;
    mqttCount = _mqttCount;

}
void DoorKeypad:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC) == 0)
    {
		// display->print("6236");	
		display->print(msg->msg);	
    }
    else
        LOG_INF("the command is not valid");
}
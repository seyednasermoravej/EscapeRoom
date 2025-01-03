#include "doorKeypad.h"
#include <cstring>
LOG_MODULE_REGISTER(doorKeypad, LOG_LEVEL_INF);
static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(door_keypad_buttons));

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
            sprintf(msg.topic, "%s/%s/button%d", instance ->roomName, instance->puzzleTypeName, (val->code - INPUT_BTN_0) + 1);
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
    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c1)));
    display8 = new Display8(DEVICE_DT_GET(DT_NODELABEL(display8)));

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
            display8->displayStr(msg->msg);
        }
        else
        {
            LOG_ERR("Not a valid index");
        }
    }
}
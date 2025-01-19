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
            sprintf(msg.topic, "%sbutton%d", instance ->mqttCommand, (val->code - INPUT_BTN_0) + 1);
            sprintf(msg.msg, "true");
            puzzleSolver('0' + (val->code - INPUT_BTN_0));
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
    sprintf(topic, "%sdisplay", mqttCommand);
    mqttList[0] = *createMqttTopic(topic);
    sprintf(topic, "%spassword", mqttCommand);
    mqttList[1] = *createMqttTopic(topic);
    sprintf(topic, "%scode", mqttCommand);
    mqttList[2] = *createMqttTopic(topic);
    mqttCount = 3;
}
void DoorKeypad:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if((strcmp("display", command) == 0))
        {
            display8->displayStr(msg->msg);
            strcpy(display, msg->msg);
            strcpy(guess, display);
            displaySet = true;
        }
        else if((strcmp("password", command) == 0))
        {
            strcpy(password, msg->msg);
            passwordSet = true;
        }
        else
        {
            LOG_ERR("Not a valid index");
        }
    }
}


void DoorKeypad:: puzzleSolver(char input)
{
    if(passwordSet && displaySet)
    {
        static uint8_t pos = 0;        
        if(input == password[pos])
        {
            guess[pos] = ' ';
            display8->displayStr(guess);
            pos++;
            if(pos == PUZZLE_DISPLAY_LEN)
            {
                pos = 0;
                struct MqttMsg msg = {0};
                sprintf(msg.topic, "%ssolved", mqttCommand);
                sprintf(msg.msg, "true");
                k_msgq_put(&msqSendToMQTT, &msg, K_FOREVER);
                LOG_INF("topic: %s, msg: %s", msg.topic, msg.msg);
            }
        }
        else
        {
            pos = 0;
            memset(guess, 32, PUZZLE_DISPLAY_LEN);//32 = char space 

            guess[PUZZLE_DISPLAY_LEN] = '\0';

            display8->displayStr(guess);
            display[PUZZLE_DISPLAY_LEN] = '\0';
            k_msleep(1000);

            memset(guess, 56, PUZZLE_DISPLAY_LEN);//56 = char 8
            display8->displayStr(guess);
            k_msleep(1000);

            memset(guess, 32, PUZZLE_DISPLAY_LEN);
            display8->displayStr(guess);
            k_msleep(1000);

            memset(guess, 56, PUZZLE_DISPLAY_LEN);//56 = char 8
            display8->displayStr(guess);
            k_msleep(1000);

            memset(guess, 32, PUZZLE_DISPLAY_LEN);
            display8->displayStr(guess);
            k_msleep(1000);

            strcpy(guess, display);
            display8->displayStr(guess);

            struct MqttMsg msg = {0};
            sprintf(msg.topic, "%ssolved", mqttCommand);
            sprintf(msg.msg, "false");
            k_msgq_put(&msqSendToMQTT, &msg, K_FOREVER);
            LOG_INF("topic: %s, msg: %s", msg.topic, msg.msg);
        }

    }
}

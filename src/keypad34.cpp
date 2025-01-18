#include "keypad34.h"


LOG_MODULE_REGISTER(keypad34, LOG_LEVEL_INF);

static Keypad34 *instance = nullptr;

void Keypad34:: handlerWrapper(struct input_event *val, void *userData)
{
    instance->handler(val);
}

void Keypad34:: handler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY) 
    {
        if(val->value)
        {
            struct MqttMsg msg = {0};
            if((val->code < INPUT_KEY_0))
            {
                sprintf(msg.topic, "%skeypad%d", prefix, val->code - INPUT_KEY_1 + 1);
            }
            if(val->code == INPUT_KEY_0)
            {
                sprintf(msg.topic, "%skeypad0", prefix);
            }
            if(val->code == INPUT_KEY_ENTER)
            {
                sprintf(msg.topic, "%skeypadEnter", prefix);
            }
            if(val->code == INPUT_KEY_ESC)
            {
                sprintf(msg.topic, "%skeypadEsc", prefix);
            }
            sprintf(msg.msg, "true");
            LOG_INF("%s",msg.topic);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
    }
}

Keypad34:: Keypad34(const char * _prefix): prefix(_prefix)
{
    device_init(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix34)));
    instance = this;
    INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(keypad34)), handlerWrapper, (void*)this);
}

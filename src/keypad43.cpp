#include "keypad43.h"


LOG_MODULE_REGISTER(keypad43, LOG_LEVEL_INF);

static Keypad43 *instance = nullptr;

void Keypad43:: handlerWrapper(struct input_event *val, void *userData)
{
    instance->handler(val);
}

void Keypad43:: handler(struct input_event *val)
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
            if(val->code == 55)
            {
                sprintf(msg.topic, "%skeypad*", prefix);
            }
            if(val->code == INPUT_KEY_E)
            {
                sprintf(msg.topic, "%skeypadHashtag", prefix);
            }
            sprintf(msg.msg, "true");
            LOG_INF("%s",msg.topic);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
    }
}

Keypad43:: Keypad43(const char * _prefix): prefix(_prefix)
{
    device_init(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix43)));
    instance = this;
    INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(keypad43)), handlerWrapper, (void*)this);
}

#include "keypad.h"


LOG_MODULE_REGISTER(keypad, LOG_LEVEL_INF);


void keypadHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_KEY) {
        struct MqttMsg msg = {0};
        strcpy(msg.topic, (char *)topic);
        // strcpy(msg.topic, KEYPAD_TOPIC);
        if(val->code < 11)
        {

            LOG_INF("code: %d, state: %s",  val->code - 1, val->value ? "pressed" : "released");
            sprintf(msg.msg, "code: %d, state: %s",  val->code - 1, val->value ? "pressed" : "released");
        }
        if(val->code == 11)
        {
            LOG_INF("code: %d, state: %s",  val->code - 2, val->value ? "pressed" : "released");
            sprintf(msg.msg, "code: %d, state: %s",  val->code - 2, val->value ? "pressed" : "released");
        }
        if(val->code == 55)
        {
            LOG_INF("*, state: %s",  val->value ? "pressed" : "released");
            sprintf(msg.msg, "*, state: %s",  val->value ? "pressed" : "released");

        }
        if(val->code == INPUT_KEY_E)
        {
            sprintf(msg.msg, "#, state: %s",  val->value ? "pressed" : "released");
            LOG_INF("#, state: %s",  val->value ? "pressed" : "released");
        }
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}

    INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(keypad)), keypadHandler, KEYPAD_TOPIC);
void keypad()
{
    device_init(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix)));
    // return 0;

}

#include "keypad.h"


LOG_MODULE_REGISTER(keypad, LOG_LEVEL_INF);


void keypadHandler(struct input_event *val, void*)
{
    if (val->type == INPUT_EV_KEY) {
        // struct MqttMsg msg = {0};
        // strcpy(msg.topic, KEYPAD_TOPIC);
        switch (val->code)
        {
            case INPUT_KEY_0:
                // strcpy(msg.msg, "key 0 is pressed");
                // LOG_INF("%s", msg.msg);

                break;
            
            default:
                break;
        }
        // printk("Key %s at row %d, column %d\n",
        printk("row %d, column %d\n",
            //    val->state ? "pressed" : "released",
               val->code / 10, val->code % 10);
    }
}

void keypad(void)
{
    const struct device *input_dev = DEVICE_DT_GET(DT_NODELABEL(kbd_matrix));
    device_init(input_dev);

    INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix)), keypadHandler, NULL);
}

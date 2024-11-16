#include "unseen.h"


LOG_MODULE_REGISTER(unseen, LOG_LEVEL_INF);


Unseen:: Unseen()
{
    keypad(DEVICE_DT_GET(DT_NODELABEL(keypad)), KEYPAD_TOPIC);
    // keypad = new Keypad(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix)), &msqSendToMQTT);
}

void Unseen:: messageHandler(struct MqttMsg *msg)
{

}


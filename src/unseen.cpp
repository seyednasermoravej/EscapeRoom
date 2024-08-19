#include "unseen.h"


LOG_MODULE_REGISTER(unseen, LOG_LEVEL_INF);

#define KEYPAD_NODE DT_NODELABEL(kbd_matrix)

Unseen:: Unseen()
{
    device_init(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix)));
    // keypad = new Keypad(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix)), &msqSendToMQTT);
}

void Unseen:: messageHandler(struct MqttMsg *msg)
{

}


#include "unseen.h"


LOG_MODULE_REGISTER(unseen, LOG_LEVEL_INF);





Unseen:: Unseen()
{
    keypad();
    display = new TftLcd(DEVICE_DT_GET(DT_CHOSEN(zephyr_display)));
}

void Unseen:: messageHandler(struct MqttMsg *msg)
{

}


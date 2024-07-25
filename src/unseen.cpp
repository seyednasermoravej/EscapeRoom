#include "unseen.h"


LOG_MODULE_REGISTER(unseen, LOG_LEVEL_INF);

static const struct device *matrixDev = INPUT_KBD_MATRIX_DT_DEFINE(DT_NODELABEL(keypad))

Unseen:: Unseen()
{
    device_init(matrixDev);
}

void Unseen:: mqttInMessageHandler(struct MqttMsg *msg)
{

}
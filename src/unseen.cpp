#include "unseen.h"


LOG_MODULE_REGISTER(unseen, LOG_LEVEL_INF);

// static const struct device *matrixDev = INPUT_KBD_MATRIX_DT_DEFINE_ROW_COL(DT_NODELABEL(keypad), 4, 4)

Unseen:: Unseen()
{
    // device_init(matrixDev);
}

void Unseen:: messageHandler(struct MqttMsg *msg)
{

}
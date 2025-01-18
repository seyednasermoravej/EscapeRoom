
#include "sticks.h"

#include <zephyr/drivers/i2c.h>

LOG_MODULE_REGISTER(sticks, LOG_LEVEL_DBG);
// #define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
//  	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
// static const struct gpio_dt_spec allSwitches[] = {
//     DT_FOREACH_PROP_ELEM(DT_NODELABEL(sticks_switch_position), gpios, DT_SPEC_AND_COMMA_GATE)
// };

static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(sticks_switch_position));


static Sticks *instance = nullptr;
// u_int8_t button_last[8];

Sticks:: Sticks(const char *room, const char *type): Puzzle(room, type)
{
    // int ret;
    // creatingMqttList(25);
    instance = this;
    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c1)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(expander1)));

    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));
    uint8_t buffer[1] = {0x00};

// static const struct i2c_dt_spec i2c_specs[] = {
//     I2C_DT_SPEC_GET(DT_NODELABEL(i2c1)),
// };
    i2c_write(i2c_dev,buffer, 1, 0x20);
    
    device_init(buttons);

    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void *)this);
}


void Sticks:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}
void Sticks:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        sprintf(msg.topic, "%sswitchPosition%d", instance->mqttCommand ,  (val->code - INPUT_BTN_0 + 1));
        val->value ? sprintf(msg.msg, "true"): sprintf(msg.msg, "false");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}


void Sticks:: creatingMqttList(uint16_t _mqttCount)
{
    mqttCount = 0;
}


void Sticks:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
}

#include "powerPanel.h"

LOG_MODULE_REGISTER(powerPanel, LOG_LEVEL_DBG);


static const struct device *const switches = DEVICE_DT_GET(DT_NODELABEL(power_panel_switches));

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),

static const struct pwm_dt_spec allServos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(power_panel_servos), pwms, DT_SPEC_AND_COMMA)
};

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(power_panel_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};
const struct gpio_dt_spec display1_8[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(power_panel_display24), gpios, DT_SPEC_AND_COMMA_GATE)
};
#define STEP PWM_USEC(100)
static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(power_panel_servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(power_panel_servos), max_pulse);
static const uint16_t servoMaxDegrees = DT_PROP(DT_NODELABEL(power_panel_servos), max_degrees);
static const uint8_t numOfDisplays = 9;


static PowerPanel *instance = nullptr;

PowerPanel:: PowerPanel(const char *room, const char *type): Puzzle(room, type)
{
    instance = this;
    device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(sevensegments24)));
    display24 = new Display24(display1_8);

    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(servo_driver)));
    servos = new Servos(allServos, ARRAY_SIZE(allServos), servoMinPulse, servoMaxPulse, servoMaxDegrees);
    for(uint8_t i = 0; i < ARRAY_SIZE(allServos); i++)
    {
        servos->move(i, 90);
    }


    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c1)));
    display4 = new Display4(DEVICE_DT_GET(DT_NODELABEL(display4)), false);

    device_init(switches);
    INPUT_CALLBACK_DEFINE(switches, switchesHandlerWrapper, (void *)this);
    int ret;

    for(unsigned int i = 0; i < ARRAY_SIZE(allRelays); i++){
        if (!device_is_ready(allRelays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&allRelays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }

    creatingMqttList(17);
}

void PowerPanel:: switchesHandlerWrapper(struct input_event *val, void *userData)
{
    instance->switchesHandler(val);
}
void PowerPanel:: switchesHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        sprintf(msg.topic, "%sswitch%d", mqttCommand, val->code - INPUT_BTN_0 + 1);
        val->value ? sprintf(msg.msg, "true"): sprintf(msg.msg, "false");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}
void PowerPanel:: creatingMqttList(uint16_t _mqttCount)
{
    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allServos); i++)
    {
        sprintf(topic, "%sservo%d", mqttCommand, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }
    for(uint8_t i = 0; i < numOfDisplays; i++)
    {
        sprintf(topic, "%sdisplay%d", mqttCommand, i + 1);
        mqttList[ARRAY_SIZE(allServos) + i] = *createMqttTopic(topic);
    }
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[ARRAY_SIZE(allServos) + numOfDisplays] = *createMqttTopic(topic);
    }
    mqttCount = ARRAY_SIZE(allServos) + numOfDisplays + ARRAY_SIZE(allRelays);
}


void PowerPanel:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);

    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strstr(command, "servo") != NULL)
        {
            char field[] = "servo";
            int commandIdx = peripheralIdx(field, command);
            uint8_t servoIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (servoIdx < ARRAY_SIZE(allServos)))
            {
                uint32_t val = atoi(msg->msg) + 90;
                if(val < 15)
                {
                    val = 15;
                }
                if(val > 165)
                {
                    val = 165;
                }
                servos->move(servoIdx, val);
                k_msleep(500);
                servos->move(servoIdx, 90);
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else if(strstr(command, "display") != NULL)
        {
            char field[] = "display";
            int commandIdx = peripheralIdx(field, command);
            uint8_t fieldIdx = commandIdx - 1;
            
            if((commandIdx > 0 ) && (fieldIdx < numOfDisplays))
            {
                if(commandIdx == 9)
                {
                    display4->displayStr(msg->msg);
                }
                else
                {
                    display24->displayStr(msg->msg, fieldIdx);
                }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else if(strstr(command, "relay") != NULL)
        {
            char field[] = "relay";
            int commandIdx = peripheralIdx(field, command);
            uint8_t relayIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
            {
                if(commandIdx == 1)
                {
                    relayOperation(msg->msg, &allRelays[relayIdx], true);
                }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else
        {
            LOG_ERR("Command not found");
        }
    }
}
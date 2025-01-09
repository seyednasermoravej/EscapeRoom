#include "xray.h"

LOG_MODULE_REGISTER(xray, LOG_LEVEL_DBG);

static const struct device *dev_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));
// static const c allRfidIns[] = {
//     PWM_DT_SPEC_GET(DT_NODELABEL(heart_servos))
// };
static const struct i2c_dt_spec i2c_specs[] = {
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid1)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid2)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid3)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid4)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid5)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid6)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid7))
};

static const struct gpio_dt_spec gpio_specs[] = {
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid1), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid2), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid3), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid4), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid5), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid6), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid7), reset_gpios, {0})
};
Xray:: Xray(const char * room, const char *type, uint8_t _numRfids): Puzzle(room, type), numRfids(_numRfids)
{
	int ret;
	ret = device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0)));
	ret = device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0)));
    // Array of devices for all mux0 channels
    const struct device *channels[] = {
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel0)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel1)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel2)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel3)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel4)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel5)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel6)),
    };

    for (uint8_t i = 0; i < ARRAY_SIZE(channels); i++) {
        if (!device_is_ready(channels[i])) {
            LOG_ERR("Channel %d is not ready", i);
            continue;
        }

        int ret = device_init(channels[i]);
        if (ret < 0) {
            LOG_ERR("Failed to initialize channel %d: %d", i, ret);
            // return ret;
        } else {
            LOG_INF("Channel %d initialized successfully", i);
        }
    }
	
	k_msleep(1);
	rfids = new Adafruit_PN532 * [numRfids];
	for (uint8_t i = 0; i < numRfids; i++) 
	{
		LOG_INF("Initializing RFID %d", i + 1);
		rfids[i] = new Adafruit_PN532(&i2c_specs[i], &gpio_specs[i]);
		k_msleep(10);
	}
	createMqttTopic(0);
    k_work_init(&cardsReaderWork, cardsReaderWorkHandler);
    k_timer_init(&cardsReaderTimer, cardsReaderTimerHandler, NULL);
    k_timer_start(&cardsReaderTimer, K_SECONDS(4), K_SECONDS(1));
}


void Xray:: creatingMqttList(uint16_t _mqttCount)
{
    mqttCount = _mqttCount;
}
void Xray:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
	LOG_INF("the command is not valid");
}

void Xray:: cardsReaderTimerHandler(struct k_timer *timer)
{
	LOG_DBG("Enterd card reader timer");
	Xray *instance = CONTAINER_OF(timer, Xray, cardsReaderTimer);
	k_work_submit(&instance->cardsReaderWork);
}
void Xray:: cardsReaderWorkHandler(struct k_work *work)
{
	bool read = false;
	char buff[17];
	LOG_DBG("Enterd card reader work");
	Xray *instance = CONTAINER_OF(work, Xray, cardsReaderWork);
	for(uint8_t i = 0; i < instance->numRfids; i++)
	{
	// uint8_t i = 0;
		instance->rfids[i]->begin();
		read = instance->rfids[i]->readCard(buff, 1000);
		if(read)
		{
			sprintf(instance->msgReader.topic, "%s/%s/rfid%d", instance->roomName, instance->puzzleTypeName, i + 1);
			sprintf(instance->msgReader.msg, "%s", buff);
			LOG_INF("The card rfid %d is : %s", i + 1, buff);
			k_msgq_put(&msqSendToMQTT, &instance->msgReader, K_NO_WAIT);
		}
		k_msleep(10);

	}

}
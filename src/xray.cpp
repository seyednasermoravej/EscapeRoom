#include "xray.h"

LOG_MODULE_REGISTER(xray, LOG_LEVEL_INF);

const struct gpio_dt_spec irq = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfidin), int_gpios, {0});
const struct gpio_dt_spec reset = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfidin), reset_gpios, {0});
const struct i2c_dt_spec rfid1I2c = I2C_DT_SPEC_GET(DT_NODELABEL(rfidin));
const struct device *dev_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));
// static const c allRfidIns[] = {
//     PWM_DT_SPEC_GET(DT_NODELABEL(heart_servos))
// };

// static Xray *instance = nullptr;
Xray:: Xray(const char * room, const char *type): Puzzle(room, type)
{
	device_init(dev_i2c);
	k_msleep(1);
	rfids = new Adafruit_PN532(&rfid1I2c, &irq, &reset);
	createMqttTopic(0);
	// instance = this;
	// char buff[17];
	// rfids->readCard(buff);
	// while(1)
	// {
	// 	instance->rfids->readCard(buff);
	// 	k_msleep(1000);

	// }
    k_work_init(&cardsReaderWork, cardsReaderWorkHandler);
    k_timer_init(&cardsReaderTimer, cardsReaderTimerHandler, NULL);
    k_timer_start(&cardsReaderTimer, K_SECONDS(4), K_SECONDS(5));
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
	LOG_INF("Enterd card reader timer");
	Xray *instance = CONTAINER_OF(timer, Xray, cardsReaderTimer);
	k_work_submit(&instance->cardsReaderWork);
}
void Xray:: cardsReaderWorkHandler(struct k_work *work)
{
	bool read = false;
	char buff[17];
	Xray *instance = CONTAINER_OF(work, Xray, cardsReaderWork);
	// instance->rfids->begin();
	read = instance->rfids->readCard(buff);
	if(read)
	{
		int idx = 1;
		struct MqttMsg msg = {0};
		sprintf(msg.topic, "%s/%s/rfid%d", instance->roomName, instance->puzzleTypeName, idx);
		sprintf(msg.msg, "%s", buff);
		LOG_INF("The card is: %s", buff);
		k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);

	}

}
#include "xray.h"

LOG_MODULE_REGISTER(xray, LOG_LEVEL_INF);

const struct gpio_dt_spec irq = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfidin), int_gpios, {0});
const struct gpio_dt_spec reset = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfidin), reset_gpios, {0});
const struct i2c_dt_spec rfid1I2c = I2C_DT_SPEC_GET(DT_NODELABEL(rfidin));
const struct device *dev_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));
// #define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	// PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
// static const c allRfidIns[] = {
//     PWM_DT_SPEC_GET(DT_NODELABEL(heart_servos))
// };


Xray:: Xray(const char * room, const char *type): Puzzle(room, type)
{
	device_init(dev_i2c);
	k_msleep(1);
	rfids = new Adafruit_PN532(&rfid1I2c, irq, reset);
	createMqttTopic(0);
while(1)
{
	char buff[17];
	rfids->readCard(buff);
	k_msleep(1000);

}
    k_timer_init(&cardsReaderTimer, cardsReader, NULL);
    k_timer_start(&cardsReaderTimer, K_SECONDS(10), K_SECONDS(5));
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

void Xray:: cardsReader(struct k_timer *timer)
{
	Xray *instance = CONTAINER_OF(timer, Xray, cardsReaderTimer);
	bool read = false;
	char buff[17];
	read = instance->rfids->readCard(buff);
	// if(read)
	// {
	// 	int idx = 0;
	// 	struct MqttMsg msg = {0};
	// 	sprintf(msg.topic, "%s/%s/rfid%d", instance->roomName, instance->puzzleTypeName, idx);
	// 	sprintf(msg.msg, "%s", buff);
	// 	LOG_INF("The card is: %s", buff);
	// 	k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);

	// }
}
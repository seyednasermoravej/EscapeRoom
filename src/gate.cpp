#include "gate.h"


LOG_MODULE_REGISTER(gate, LOG_LEVEL_INF);

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),


//  //const struct gpio_dt_spec spec = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(leds), gpios, 1);
static const struct gpio_dt_spec addresses[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(addresses), gpios, DT_SPEC_AND_COMMA_GATE)
};


#define HINT_NODE	DT_ALIAS(hintbutton)
#if !DT_NODE_HAS_STATUS(HINT_NODE, okay)
#error "Unsupported board: HINT devicetree alias is not defined"
#endif


static const struct gpio_dt_spec hintButton = GPIO_DT_SPEC_GET_OR(HINT_NODE, gpios,
							      {0});


#define RELAYS_CHECK_NODE   DT_NODELABEL(relays_check)
static const struct gpio_dt_spec relaysCheck = GPIO_DT_SPEC_GET_OR(RELAYS_CHECK_NODE, gpios, {0});

#define MAGLOCKS_NODE   DT_NODELABEL(magloccks)
static const struct gpio_dt_spec magLocks = GPIO_DT_SPEC_GET_OR(MAGLOCKS_NODE, gpios, {0});

#define RFID_OUT_LED_NODE   DT_NODELABEL(rfid_led)
static const struct gpio_dt_spec rfidOutLed = GPIO_DT_SPEC_GET_OR(RFID_OUT_LED_NODE, gpios, {0});

#define RFID_OUT_BUZZER_NODE   DT_NODELABEL(buzzer)
static const struct gpio_dt_spec rfidOutBuzzer = GPIO_DT_SPEC_GET_OR(RFID_OUT_BUZZER_NODE, gpios, {0});

static struct gpio_callback hintButton_cb_data;
static struct gpio_callback relaysCheck_cb_data;
static struct gpio_callback maglocks_cb_data;


void hintPressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
    // k_msgq_put(&msqSendToMQTT, buf, K_NO_WAIT);

}

void relaysStatusChange(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

	LOG_INF("Relays  status changed. %" PRIu32 "\n", k_cycle_get_32());
    // k_msgq_put(&msqSendToMQTT, buf, K_NO_WAIT);

}
void magLocksStatusChange(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

	LOG_INF("mag locks status changed. %" PRIu32 "\n", k_cycle_get_32());
    // k_msgq_put(&msqSendToMQTT, buf, K_NO_WAIT);

}
Gate:: Gate()
{
    hintButtonInit();
    lcdThreadCreate();
    rfidInThreadCreate();
	addrKeysInit();
	relaysCheckInit();
	magLocksInit();
}



void Gate:: sendDevAddrVal()
{
    int dev_address = 0;
    char dev_addr_char[5];
    dev_address=addrKeysVal();
    sprintf(dev_addr_char,"%d",dev_address);
    LOG_INF("The Device Address is %d\n\r",dev_address);
    struct MqttMsg send = {0};
    strcpy(send.topic, DEV_ADDR_TOPIC);
    strcpy(send.msg, dev_addr_char);
    k_msgq_put(&msqSendToMQTT, &send, K_NO_WAIT);
}


int Gate:: addrKeysVal()
{
    int addr1=0;
    int addr2=0;
    int addr4=0;
    int addr8=0;
    int addr16=0;
    int addr32=0;
    int address_integrated=0;
    addr1=gpio_pin_get_dt(&addresses[0]);
    // printk("The Device Addr1 is %d\n\r",addr1);
    addr2=gpio_pin_get_dt(&addresses[1]);
    // printk("The Device Addr2 is %d\n\r",addr2);
    addr4=gpio_pin_get_dt(&addresses[2]);
    // printk("The Device Addr4 is %d\n\r",addr4);
    addr8=gpio_pin_get_dt(&addresses[3]);
    // printk("The Device Addr8 is %d\n\r",addr8);
    addr16=gpio_pin_get_dt(&addresses[4]);
    // printk("The Device Addr16 is %d\n\r",addr16);
    addr32=gpio_pin_get_dt(&addresses[5]);
    // printk("The Device Addr32 is %d\n\r",addr32);
    address_integrated = addr1 + (addr2*2) + (addr4*4) + (addr8*8) + (addr16*16) + (addr32*32);
    return address_integrated;
}

int Gate:: addrKeysInit()
{
    int ret;

    for(int i = 0; i < ARRAY_SIZE(addresses); i++){
        if (!device_is_ready(addresses[i].port)) {
		    return -1;
	    }
        ret = gpio_pin_configure_dt(&addresses[i], GPIO_INPUT);
	    if (ret < 0) {
		    return -1;
	    }
    }
	return 0;
}

int Gate:: hintButtonInit()
{
	device_init(hintButton.port);
    int ret;
	if (!gpio_is_ready_dt(&hintButton)) {
		printk("Error: button device %s is not ready\n",
		       hintButton.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&hintButton, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, hintButton.port->name, hintButton.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&hintButton,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, hintButton.port->name, hintButton.pin);
		return 0;
	}

	gpio_init_callback(&hintButton_cb_data, hintPressed, BIT(hintButton.pin));
	gpio_add_callback(hintButton.port, &hintButton_cb_data);
	LOG_INF("Set up button at %s pin %d\n", hintButton.port->name, hintButton.pin);
	return ret;
}



void Gate:: messageHandler(struct MqttMsg *msg)
{

    if(strcmp(msg->topic, LCD_TOPIC) == 0)
    {
        struct LcdMsg lcd = {0};
        strcpy(lcd.firstLine, msg->msg);
        k_msgq_put(&msqLcd1, &lcd, K_NO_WAIT);
        k_msgq_put(&msqLcd2, &lcd, K_NO_WAIT);
    }
}

int Gate:: relaysCheckInit()
{
	device_init(relaysCheck.port);
    int ret;
	if (!gpio_is_ready_dt(&relaysCheck)) {
		printk("Error: button device %s is not ready\n",
		       relaysCheck.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&relaysCheck, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, relaysCheck.port->name, relaysCheck.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&relaysCheck,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, relaysCheck.port->name, relaysCheck.pin);
		return 0;
	}

	gpio_init_callback(&relaysCheck_cb_data, relaysStatusChange, BIT(relaysCheck.pin));
	gpio_add_callback(relaysCheck.port, &relaysCheck_cb_data);
	LOG_INF("Set up button at %s pin %d\n", relaysCheck.port->name, relaysCheck.pin);
	return ret;
}

int Gate:: magLocksInit()
{

    int ret;
	if (!gpio_is_ready_dt(&magLocks)) {
		printk("Error: button device %s is not ready\n",
		       magLocks.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&magLocks, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, magLocks.port->name, magLocks.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&magLocks,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, magLocks.port->name, magLocks.pin);
		return 0;
	}

	gpio_init_callback(&maglocks_cb_data, magLocksStatusChange, BIT(magLocks.pin));
	gpio_add_callback(magLocks.port, &maglocks_cb_data);
	LOG_INF("Set up maglocks at %s pin %d\n", magLocks.port->name, magLocks.pin);
	return ret;
}
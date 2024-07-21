#include "gate.h"


LOG_MODULE_REGISTER(gate, LOG_LEVEL_INF);

#define DT_SPEC_AND_COMMA1(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),


//  //const struct gpio_dt_spec spec = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(leds), gpios, 1);
static const struct gpio_dt_spec addresses[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(addresses), gpios, DT_SPEC_AND_COMMA1)
};


#define HINT_NODE	DT_ALIAS(hintbutton)
#if !DT_NODE_HAS_STATUS(HINT_NODE, okay)
#error "Unsupported board: HINT devicetree alias is not defined"
#endif


static const struct gpio_dt_spec hintButton = GPIO_DT_SPEC_GET_OR(HINT_NODE, gpios,
							      {0});


#define RFID_OUT_LED_NODE   DT_NODELABEL(rfid_led)
static const struct gpio_dt_spec rfidOutLed = GPIO_DT_SPEC_GET_OR(RFID_OUT_LED_NODE, gpios, {0});


#define RFID_OUT_BUZZER_NODE   DT_NODELABEL(buzzer)
static const struct gpio_dt_spec rfidOutBuzzer = GPIO_DT_SPEC_GET_OR(RFID_OUT_BUZZER_NODE, gpios, {0});

static struct gpio_callback hintButton_cb_data;
void hintPressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
    // k_msgq_put(&msqSendToMQTT, buf, K_NO_WAIT);

}
Gate:: Gate()
{
    hintButtonInit();
    lcdThreadCreate();
    rfidInThreadCreate();

}



void Gate:: sendDevAddrVal()
{
    int dev_address = 0;
    char dev_addr_char[5];
    dev_address=addrKeysVal();
    sprintf(dev_addr_char,"%d",dev_address);
    printk("The Device Address is %d\n\r",dev_address);
    struct MqttMsg *send = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));
    memset(send, 0, sizeof(struct MqttMsg));
    strcpy(send->topic, "sub/devAddr");
    strcpy(send->msg, dev_addr_char);
    k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
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

    for(int i=0; i<6; i++){
        if (!device_is_ready(addresses[i].port)) {
		    return -1;
	    }
        ret = gpio_pin_configure_dt(&addresses[i], GPIO_INPUT);
	    if (ret < 0) {
		    return -1;
	    }
    }
}

int Gate:: hintButtonInit()
{

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
	printk("Set up button at %s pin %d\n", hintButton.port->name, hintButton.pin);
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
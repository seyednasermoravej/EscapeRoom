#include "main.h"
#include <stdlib.h>
#include <zephyr/drivers/i2c.h>
#include "lcd.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);
// #define DT_SPEC_AND_COMMA(node_id, prop, idx) \
// 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),


// static const struct gpio_dt_spec allAddress[] = {
//     DT_FOREACH_PROP_ELEM(DT_NODELABEL(address), gpios, DT_SPEC_AND_COMMA)
// };
#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led00 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);




static const struct gpio_dt_spec address = GPIO_DT_SPEC_GET_OR(ADDR_NODE, gpios,
							      {0});

int threadsCreation();

extern void puzzleThreadCreate();

int main()
{ 
    //performing new update on firmware

    // messageQueueInit();
    

    // dhcpClient();
    // //http request for getting DFU
    // sem_wait(&dhcpActive);
    // sem_destroy(&dhcpActive);


    const struct device *i2c_dev;
    uint8_t addr;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printf("Error: I2C device not found\n");
        return;
    }

    printf("Scanning I2C bus...\n");
// while (1)
// {


//     for (addr = 1; addr < 128; addr++) {
//         struct i2c_msg msg;
//         uint8_t dummy_data = 0;

//         msg.buf = &dummy_data;
//         msg.len = 1;
//         msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

//         ret = i2c_transfer(i2c_dev, &msg, 1, addr);
//         if (ret == 0) {
//             printf("Device found at address 0x%02X\n", addr);
//         }
//     }
//     k_msleep(1000);
// }
    lcdInit();


















    // int ret;
    // 	if (!gpio_is_ready_dt(&allAddress[1])) {
	// 	printk("Error: address device %s is not ready\n",
	// 	       allAddress[1].port->name);
	// 	return 0;
	// }
    // 	ret = gpio_pin_configure_dt(&allAddress[1], GPIO_INPUT);
	// if (ret != 0) {
	// 	printk("Error %d: failed to configure %s pin %d\n",
	// 	       ret, allAddress[1].port->name, allAddress[1].pin);
	// 	return 0;
	// }
    	if (!gpio_is_ready_dt(&address)) {
		printk("Error: address device %s is not ready\n",
		       address.port->name);
		return 0;
	}
    	ret = gpio_pin_configure_dt(&address, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, address.port->name, address.pin);
		return 0;
	}

	if (!gpio_is_ready_dt(&led00)) {
		return;
	}
    	ret = gpio_pin_configure_dt(&led00, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

    while (1)
    {
        // uint8_t addr = gpio_pin_get_dt(&address);
        // // uint8_t addr = gpio_pin_get_dt(&allAddress[1]);
        // LOG_INF("address 0 value: %d", addr);
        		ret = gpio_pin_toggle_dt(&led00);
		if (ret < 0) {
			return;
		}
        k_msleep(1000);
    }
    threadsCreation();
    char *send = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
    memset(send, 0, MESSAGE_QUEUE_LEN);
    strcpy(send, "allah");
    char buf[4];
    int counter = 0;
    while(1)
    {
        // strcat(send, itoa(counter, buf, 10));
        // counter++;
        int ret = k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
        k_msleep(2000);
    }
    // return 0;
}


int threadsCreation()
{
    mqttThreadCreate();
    puzzleThreadCreate();
}
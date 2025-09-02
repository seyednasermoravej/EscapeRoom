#include "main.h"




#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_NODELABEL(motherboard_audio_speaker));
// #define MIN_PERIOD PWM_SEC(1U) / 128U
// #define MAX_PERIOD PWM_SEC(1U)


void i2cScanner();

void test(void) {
   	// uint32_t max_period;
	// uint32_t period;
	// uint8_t dir = 0U;
	// int ret;

	// printk("PWM-based blinky\n");

	// if (!pwm_is_ready_dt(&pwm_led0)) {
	// 	printk("Error: PWM device %s is not ready\n",
	// 	       pwm_led0.dev->name);
	// 	return 0;
	// }

	// /*
	//  * In case the default MAX_PERIOD value cannot be set for
	//  * some PWM hardware, decrease its value until it can.
	//  *
	//  * Keep its value at least MIN_PERIOD * 4 to make sure
	//  * the sample changes frequency at least once.
	//  */
	// printk("Calibrating for channel %d...\n", pwm_led0.channel);
	// max_period = MAX_PERIOD;
	// while (pwm_set_dt(&pwm_led0, max_period, max_period / 2U)) {
	// 	max_period /= 2U;
	// 	if (max_period < (4U * MIN_PERIOD)) {
	// 		printk("Error: PWM device "
	// 		       "does not support a period at least %lu\n",
	// 		       4U * MIN_PERIOD);
	// 		return 0;
	// 	}
	// }

	// printk("Done calibrating; maximum/minimum periods %u/%lu nsec\n",
	//        max_period, MIN_PERIOD);

	// period = max_period;
	// while (1) {
	// 	ret = pwm_set_dt(&pwm_led0, period, period / 2U);
	// 	if (ret) {
	// 		printk("Error %d: failed to set pulse width\n", ret);
	// 		return 0;
	// 	}
	// 	printk("Using period %d\n", period);

	// 	period = dir ? (period * 2U) : (period / 2U);
	// 	if (period > max_period) {
	// 		period = max_period / 2U;
	// 		dir = 0U;
	// 	} else if (period < MIN_PERIOD) {
	// 		period = MIN_PERIOD * 2U;
	// 		dir = 1U;
	// 	}

	// 	k_sleep(K_SECONDS(4U));
	// }
	// return 0;
}

// struct mqtt_topic deviceId_topic = {0};
extern void puzzleThreadCreate();
char deviceId[17]; // Each byte is 2 hex digits, plus null terminator
uint8_t devId[8];
char deviceIdPub[32];
void readingHWinfo(char *idStr);

#if defined(CONFIG_USB_DEVICE_STACK_NEXT)
static struct usbd_context *sample_usbd;

static int enable_usb_device_next(void)
{
	int err;

	sample_usbd = sample_usbd_init_device(NULL);
	if (sample_usbd == NULL) {
		return -ENODEV;
	}

	err = usbd_enable(sample_usbd);
	if (err) {
		return err;
	}

	return 0;
}
SYS_INIT(enable_usb_device_next, APPLICATION, 50);
#endif /* defined(CONFIG_USB_DEVICE_STACK_NEXT) */


int wdt_channel_id;
const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));


int main()
{
    LOG_INF("besme allah");
    readingHWinfo(deviceId);
    LOG_INF("Device ID: %s", deviceId);
   test();
    puzzleThreadCreate();

}

void i2cScanner()
{

    const struct device *i2c_dev;
    uint8_t addr;
    int ret;
    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0)));
    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!i2c_dev) {
        printf("Error: I2C device not found\n");
        return;
    }

    printf("Scanning I2C bus...\n");
    struct MqttMsg mqtt = {0};
    strcpy(mqtt.topic, "pub/");
    strcat(mqtt.topic, deviceId);
    bool deviceFound = false;
    while (1)
    {

        deviceFound = false;
        for (addr = 1; addr < 128; addr++) {
            struct i2c_msg msg;
            uint8_t dummy_data = 0;

            msg.buf = &dummy_data;
            msg.len = 1;
            msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

            ret = i2c_transfer(i2c_dev, &msg, 1, addr);

            if (ret == 0) {
                printf("Device found at address 0x%02X\n", addr);
                sprintf(mqtt.msg, "Device found at address 0x%02X\n", addr);
                k_msgq_put(&msqSendToMQTT, &mqtt, K_NO_WAIT);
                deviceFound = true;
            }
        }
        if(!deviceFound)
        {
            sprintf(mqtt.msg, "device not found.");
            k_msgq_put(&msqSendToMQTT, &mqtt, K_NO_WAIT);
            LOG_INF("device not found.");

        }
        k_msleep(4000);
    }
}

// void test()
// {
//     // struct MqttMsg mg = {0};
//     // sprintf(msg.msg, "Button 0 pressed");

//    i2cScanner();
//     while(1)
//     {

//         k_msleep(1000);

//     }
// }


void readingHWinfo(char *idStr)
{
    ssize_t length;

    length = hwinfo_get_device_id(devId, sizeof(devId));

    if (length > 0) {
        // LOG_PRINTK("Device ID: ");
        for (int i = 0; i < length; i++) {
            // LOG_PRINTK("%02x", id[i]);
            sprintf(&idStr[i * 2], "%02x", devId[i]);
        }
        idStr[2 * length] = '\0';
        // LOG_PRINTK("\n");
    } else {
        LOG_PRINTK("Failed to read device ID\n");
    }
}


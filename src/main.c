#include "main.h"
#include <stdlib.h>
#include <zephyr/drivers/i2c.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void threadsCreation();
void i2cScanner();
void test();

extern void puzzleThreadCreate();
extern void mqttThreadCreate();

int main()
{ 
    //performing new update on firmware

    // test();
    printk("hi");
    LOG_INF("besme allah");
    dhcpClient();
    //http request for getting DFU
    // sem_wait(&dhcpActive);
    // sem_destroy(&dhcpActive);

    threadsCreation();
    struct MqttMsg *send = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));
    memset(send, 0, sizeof(struct MqttMsg));
    strcpy(send->topic, "pub/escape");
    strcpy(send->msg, "hi Bram and Philippe");
    k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
    while(1)
    {
        k_msleep(2000);
        k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
    }
}


void threadsCreation()
{
    mqttThreadCreate();
    puzzleThreadCreate();
}

void i2cScanner()
{

    const struct device *i2c_dev;
    uint8_t addr;
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printf("Error: I2C device not found\n");
        return;
    }

    printf("Scanning I2C bus...\n");
    while (1)
    {


        for (addr = 1; addr < 128; addr++) {
            struct i2c_msg msg;
            uint8_t dummy_data = 0;

            msg.buf = &dummy_data;
            msg.len = 1;
            msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

            ret = i2c_transfer(i2c_dev, &msg, 1, addr);
            if (ret == 0) {
                printf("Device found at address 0x%02X\n", addr);
            }
        }
        k_msleep(1000);
    }
}


void test()
{
    while(1)
    {
        LOG_INF("inf");
        LOG_DBG("dbg");
        LOG_ERR("error");
        k_msleep(1000);
    }
    puzzleThreadCreate();
}
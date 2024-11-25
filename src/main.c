#include "main.h"

#include "keypad.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

void i2cScanner();
void test();
struct mqtt_topic deviceId_topic = {0};
extern void puzzleThreadCreate();
extern void mqttThreadCreate(char *);
char deviceId[17]; // Each byte is 2 hex digits, plus null terminator
char deviceIdPub[32];
void readingHWinfo(char *idStr);

int main()
{ 
    //performing new update on firmware
    // sys_trace_sys_init_enter();
    LOG_INF("besme allah");
    readingHWinfo(deviceId);
    LOG_INF("Device ID: %s", deviceId);
    sprintf(deviceIdPub, "sub/%s", deviceId);
    // dhcpClient();

    deviceId_topic.topic.utf8 = deviceIdPub;
    deviceId_topic.topic.size = strlen(deviceIdPub);
    deviceId_topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    // test();

    // char serverName[] = "mqtt-1.localdomain";
    char serverName[] = "test.mosquitto.org";
    char serverIpAddress[128] = {0};
    dnsResolver(serverName, serverIpAddress);
    
    //http request for getting DFU
    
    // sem_destroy(&dhcpActive);
    mqttThreadCreate(serverIpAddress);


    puzzleThreadCreate();

    struct MqttMsg *send = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));
    memset(send, 0, sizeof(struct MqttMsg));
    // strcpy(send->topic, "pub/");
    // strcat(send->topic, deviceId);
    // // strcpy(send->msg, "Hello guys.");
    // k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
    while(1)
    {
        k_sleep(K_SECONDS(120));
        // sprintf(send->msg, "escape room %d", sys_clock_cycle_get_32());
        // k_msgq_put(&msqSendToMQTT, send, K_NO_WAIT);
    }
}

void i2cScanner()
{

    const struct device *i2c_dev;
    uint8_t addr;
    int ret;
    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c1)));
    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));
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

void test()
{
    // struct MqttMsg mg = {0};
    // sprintf(msg.msg, "Button 0 pressed");

    i2cScanner();
    while(1)
    {
        k_msleep(1000);
    }
}


void readingHWinfo(char *idStr)
{
    uint8_t id[8];
    ssize_t length;

    length = hwinfo_get_device_id(id, sizeof(id));

    if (length > 0) {
        // LOG_PRINTK("Device ID: ");
        for (int i = 0; i < length; i++) {
            // LOG_PRINTK("%02x", id[i]);
            sprintf(&idStr[i * 2], "%02x", id[i]);
        }
        idStr[2 * length] = '\0';
        // LOG_PRINTK("\n");
    } else {
        LOG_PRINTK("Failed to read device ID\n");
    }
}
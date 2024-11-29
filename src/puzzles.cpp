#include "puzzles.h"
#include "topics.h"
LOG_MODULE_REGISTER(puzzle, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(puzzleStackArea, PUZZLE_STACK_SIZE);

struct k_thread puzzleThread;

static const struct gpio_dt_spec builtInLed = GPIO_DT_SPEC_GET_OR(BUILT_IN_NODE, gpios,
							      {0});


extern void mqttThreadCreate(char *, struct mqtt_topic *mqttList, uint16_t mqttCount);
static struct nvs_fs fileSystem;

Puzzles::Puzzles(struct nvs_fs *_fs): fs(_fs)
{
    nvsInit();
    builtIntLedInit();
    readInfosFromMemory();
    // puzzleTypeSelection("config");
    // puzzleTypeSelection("rotating platform");
}



Puzzles *puzzles = nullptr;

void Puzzles:: puzzleTypeSelection(char *type)
{
    struct MqttMsg msg = {0};
    strcpy(msg.topic, "pub/");
    strcat(msg.topic, deviceId);

    if(strcmp(type, "console") == 0)
    {
        sprintf(msg.msg, "Puzzle type is console");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Console("introRoom", "console");
        LOG_INF("puzzle type is console.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "platform") == 0)
    {
        sprintf(msg.msg, "Puzzle type is platform");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Platform("introRoom", "platform");
        LOG_INF("Puzzle type is Platform.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "cabinet") == 0)
    {
        sprintf(msg.msg, "Puzzle type is cabinet");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Cabinet("introRoom", "cabinet");
        LOG_INF("Puzzle type is cabinet.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "door") == 0)
    {
        sprintf(msg.msg, "Puzzle type is door");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Door("codeRed", "door");
        LOG_INF("Puzzle type is Door.");
        deviceSpecified = true;
    }
    // else if(strcmp(type, "shelf") == 0)
    // {
    //     sprintf(msg.msg, "Puzzle type is shelf");
    //     k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    //     puzzle = new Shelf("codeRed", "shelf");
    //     LOG_INF("Puzzle type is shelf.");
    //     deviceSpecified = true;
    // }
    else if(strcmp(type, "doorKeypad") == 0)
    {
        sprintf(msg.msg, "Puzzle type is door keypad");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Door("codeRed", "doorKeypad");
        LOG_INF("Puzzle type is door keypad.");
        deviceSpecified = true;
    }
    // else if(strcmp(type, "xray") == 0)
    // {
    //     sprintf(msg.msg, "Puzzle type is xray");
    //     k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    //     puzzle = new Xray("codeRed", "xray");
    //     LOG_INF("Puzzle type is xray.");
    //     deviceSpecified = true;
    // }
    else if(strcmp(type, "hartMonitor") == 0)
    {
        sprintf(msg.msg, "Puzzle type is hartMonitor");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new HartMonitor("codeRed", "hartMonitor");
        LOG_INF("Puzzle type is hartMonitor.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "vantilator") == 0)
    {
        sprintf(msg.msg, "Puzzle type is vantilator");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Ventilator("codeRed", "vantilator");
        LOG_INF("Puzzle type is vantilator.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "fridge") == 0)
    {
        sprintf(msg.msg, "Puzzle type is fridge");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Fridge("codeRed", "fridge");
        LOG_INF("Puzzle type is fridge.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "blinds") == 0)
    {
        sprintf(msg.msg, "Puzzle type is blinds");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        puzzle = new Blinds("codeRed", "blinds");
        LOG_INF("Puzzle type is blinds");
        deviceSpecified = true;
    }
    // else if(strcmp(type, "powerPanel") == 0)
    // {
    //     sprintf(msg.msg, "Puzzle type is powerPanel");
    //     k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    //     puzzle = new PowerPanel("codeRed", "powerPanel");
    //     LOG_INF("Puzzle type is powerPanel");
    //     deviceSpecified = true;
    // }

    if(deviceSpecified)
    {
        gpio_pin_set_dt(&builtInLed, 1);
    }
    else
    {
        deviceSpecified = false;
        LOG_INF("Puzzle type is not recognized.");
        gpio_pin_set_dt(&builtInLed, 0);
    }
}


void Puzzles:: messageHandler(struct MqttMsg *msg)
{
    if(!deviceSpecified)
    {
        if(strcmp(msg->topic, PUZZLE_TYPE_TOPIC) == 0)
        {
            writeDeviceName(msg->msg);
            deviceSpecified = true;
            puzzleTypeSelection(msg->msg);
        }
    }
    else
    {
        
        if(strcmp(msg->topic, BUILT_IN_LED_TOPIC) == 0)
        {   
            int ret;
            if(strncmp("0", msg->msg, 1) == 0)
            {
                ret = gpio_pin_set_dt(&builtInLed, 0);
                LOG_INF("Built in led deactivate");
            }
            else if(strncmp("1", msg->msg, 1) == 0)
            {
                ret = gpio_pin_set_dt(&builtInLed, 1);
                LOG_INF("Built in led activate");
            }
            else
            {
                LOG_INF("Command is not recognized");
            }
        }
        else
        {
            puzzle->messageHandler(msg);
        }

    }

}


void Puzzles:: alive()
{
    puzzle->alive();
}
int Puzzles:: nvsInit()
{
    int rc;
    struct flash_pages_info info;
    fs->flash_device = NVS_PARTITION_DEVICE;
    	if (!device_is_ready(fs->flash_device)) {
		printk("Flash device %s is not ready\n", fs->flash_device->name);
		return 0;
	}
	fs->offset = NVS_PARTITION_OFFSET;
	rc = flash_get_page_info_by_offs(fs->flash_device, fs->offset, &info);
	if (rc) {
		printk("Unable to get page info, rc=%d\n", rc);
		return 0;
	}
	fs->sector_size = info.size;
	fs->sector_count = 3U;
    rc = nvs_mount(fs);
	if (rc) {
        flash_erase(fs->flash_device, NVS_PARTITION_OFFSET, 0x100000);
        rc = nvs_mount(fs);
        if (rc) {
            printk("Flash Init failed, rc=%d\n", rc);
            return 0;
        }
	}
    return 0;
}

void Puzzles:: readInfosFromMemory()
{
    // nvs_delete(fs, 0);
    // gpio_pin_set_dt(&builtInLed, 1);
    // while(1);




    int rc = 0;
    char name[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    rc = nvs_read(fs, 0, &name, PUZZLE_TYPE_NAME_MAX_LEN);
    if(rc > 0)
    {
        // deviceSpecified = true;
        puzzleTypeSelection(name);
    }
}

int Puzzles:: builtIntLedInit()
{
    int ret;
	if (!gpio_is_ready_dt(&builtInLed)) {
		printk("Error: button device %s is not ready\n",
		       builtInLed.port->name);
		return 0;
	}
    ret = gpio_pin_configure_dt(&builtInLed, GPIO_OUTPUT_INACTIVE);

	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, builtInLed.port->name, builtInLed.pin);
		return 0;
	}
    return ret;
}

void puzzleEntryPoint(void *serverIpAddress, void *, void *)
{


    struct MqttMsg *msg = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));

    memset(msg, 0, sizeof(struct MqttMsg));
    puzzles = new Puzzles(&fileSystem);
    bool mqtt = false;

    while(!puzzles->deviceSpecified)
    {
        if(!mqtt)
        { 
            mqttThreadCreate((char*)serverIpAddress, &puzzleType_topic, 1);
            mqtt = true;
        }
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            puzzles -> messageHandler(msg);
            memset(msg, 0, sizeof(struct MqttMsg));
            if(puzzles->deviceSpecified)
                sys_reboot(0);
        
        }
        k_msleep(1000);

    }


    mqttThreadCreate((char*)serverIpAddress, puzzles->puzzle->getMqttList(), puzzles->puzzle->getMqttCount());
    int counter = 0; 
    while(1)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            puzzles -> messageHandler(msg); 
        }
        counter++;
        if(counter > 5)
        {
            puzzles -> alive();  
            counter = 0;
        }
        k_msleep(1000);
    }
}

int Puzzles:: writeDeviceName(char *name)
{
    char buf[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    char buf2[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    strcpy(buf, name);
    nvs_write(fs, 0, &buf, PUZZLE_TYPE_NAME_MAX_LEN + 1);
    nvs_read(fs, 0, &buf2, PUZZLE_TYPE_NAME_MAX_LEN);
}

extern "C" void puzzleThreadCreate(char *serverIpAddress)
{
    k_tid_t puzzleTid = k_thread_create(&puzzleThread, puzzleStackArea,
									K_THREAD_STACK_SIZEOF(puzzleStackArea),
									puzzleEntryPoint, (void *)serverIpAddress, NULL, NULL,
									PUZZLE_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(puzzleTid, "puzzles");
}



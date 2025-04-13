#include "puzzles.h"
#include "topics.h"

LOG_MODULE_REGISTER(puzzles, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(puzzleStackArea, PUZZLE_STACK_SIZE);

struct k_thread puzzleThread;

#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#else
static const struct gpio_dt_spec builtInLed = GPIO_DT_SPEC_GET_OR(BUILT_IN_NODE, gpios,
							      {0});
#endif
extern void mqttThreadCreate(char *, struct mqtt_topic *mqttList, uint16_t mqttCount);
struct nvs_fs fileSystem;



Puzzles::Puzzles(struct nvs_fs *_fs): fs(_fs)
{
    nvsInit();

#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#else
    builtIntLedInit();
#endif
    readInfosFromMemory();
    // puzzleTypeSelection("config");
    // puzzleTypeSelection("rotating platform");
}



    // device_init(tcs34725_dev);



Puzzles *puzzles = nullptr;

void Puzzles:: puzzleTypeSelection(char *type)
{


    // struct MqttMsg msg = {0};
    // strcpy(msg.topic, "pub/");
    // strcat(msg.topic, deviceId);

    if(strcmp(type, "console") == 0)
    {
        puzzle = new Console("introRoom", "console");
        LOG_INF("puzzle type is console.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "platform") == 0)
    {
        puzzle = new Platform("introRoom", "platform");
        LOG_INF("Puzzle type is Platform.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "cabinet") == 0)
    {
        puzzle = new Cabinet("introRoom", "cabinet");
        LOG_INF("Puzzle type is cabinet.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "door") == 0)
    {
        puzzle = new Door("codeRed", "door");
        LOG_INF("Puzzle type is Door.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "shelf") == 0)
    {
        puzzle = new Xray("codeRed", "shelf", 4);
        LOG_INF("Puzzle type is shelf.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "doorKeypad") == 0)
    {
        puzzle = new DoorKeypad("codeRed", "doorKeypad");
        LOG_INF("Puzzle type is door keypad.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "xray") == 0)
    {
        puzzle = new Xray("codeRed", "xray", 7);
        LOG_INF("Puzzle type is xray.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heartMonitor") == 0)
    {
        puzzle = new HeartMonitor("codeRed", "heartMonitor");
        LOG_INF("Puzzle type is heartMonitor.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "ventilator") == 0)
    {
        puzzle = new Ventilator("codeRed", "ventilator");
        LOG_INF("Puzzle type is ventilator.");
        deviceSpecified = true;
    }
#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#else
    else if(strcmp(type, "fridge") == 0)
    {
        puzzle = new Fridge("codeRed", "fridge");
        LOG_INF("Puzzle type is fridge.");
        deviceSpecified = true;
    }
#endif
    else if(strcmp(type, "blinds") == 0)
    {
        puzzle = new Blinds("codeRed", "blinds");
        LOG_INF("Puzzle type is blinds");
        deviceSpecified = true;
    }
    else if(strcmp(type, "powerPanel") == 0)
    {
        puzzle = new PowerPanel("codeRed", "powerPanel");
        LOG_INF("Puzzle type is powerPanel");
        deviceSpecified = true;
    }
    else if(strcmp(type, "scale") == 0)
    {
        puzzle = new Scale("codeRed", "scale");
        LOG_INF("Puzzle type is scale");
        deviceSpecified = true;
    }
    else if(strcmp(type, "colorTubes") == 0)
    {
        puzzle = new colorTubes("codeRed", "colorTubes", 15);
        LOG_INF("Puzzle type is colorTubes.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "sticks") == 0)
    {
        puzzle = new Sticks("codeRed", "sticks");
        LOG_INF("Puzzle type is sticks.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "entranceDoor") == 0)
    {
        puzzle = new EntranceDoor("codeRed", "entranceDoor");
        LOG_INF("Puzzle type is entranceDoor");
        deviceSpecified = true;
    }
    else if(strcmp(type, "exitDoor") == 0)
    {
        puzzle = new ExitDoor("codeRed", "exitDoor");
        LOG_INF("Puzzle type is exitDoor");
        deviceSpecified = true;
    }
    else if(strcmp(type, "pneumaPost") == 0)
    {
        puzzle = new PneumaPost("codeRed", "pneumaPost");
        LOG_INF("Puzzle type is pneumaPost");
        deviceSpecified = true;
    }
    else if(strcmp(type, "defib") == 0)
    {
        puzzle = new Defib("codeRed", "defib");
        LOG_INF("Puzzle type is defib");
        deviceSpecified = true;
    }
    else if(strcmp(type, "drawers") == 0)
    {
        puzzle = new Drawers("codeRed", "drawers");
        LOG_INF("Puzzle type is drawers");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heartBox1") == 0)
    {
        puzzle = new HeartBox("codeRed", "heartBox1");
        LOG_INF("Puzzle type is heartBox1");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heartBox2") == 0)
    {
        puzzle = new HeartBox("codeRed", "heartBox2");
        LOG_INF("Puzzle type is heartBox2");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heartBox3") == 0)
    {
        puzzle = new HeartBox("codeRed", "heartBox3");
        LOG_INF("Puzzle type is heartBox3");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heartBox4") == 0)
    {
        puzzle = new HeartBox("codeRed", "heartBox4");
        LOG_INF("Puzzle type is heartBox4");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heart1") == 0)
    {
        puzzle = new Heart("codeRed", "heart1");
        LOG_INF("Puzzle type is heart1");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heart2") == 0)
    {
        puzzle = new Heart("codeRed", "heart2");
        LOG_INF("Puzzle type is heart2");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heart3") == 0)
    {
        puzzle = new Heart("codeRed", "heart3");
        LOG_INF("Puzzle type is heart3");
        deviceSpecified = true;
    }
    else if(strcmp(type, "heart4") == 0)
    {
        puzzle = new Heart("codeRed", "heart4");
        LOG_INF("Puzzle type is heart4");
        deviceSpecified = true;
    }

    if(deviceSpecified)
    {
        #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        #else
            gpio_pin_set_dt(&builtInLed, 1);
        #endif
    }
    else
    {
        deviceSpecified = false;
        LOG_INF("Puzzle type is not recognized.");
        #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        #else
            gpio_pin_set_dt(&builtInLed, 0);
        #endif
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
            if(strncmp("0", msg->msg, 1) == 0)
            {
        #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        #else
            gpio_pin_set_dt(&builtInLed, 0);
        #endif
                LOG_INF("Built in led deactivate");
            }
            else if(strncmp("1", msg->msg, 1) == 0)
            {
        #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        #else
            gpio_pin_set_dt(&builtInLed, 1);
        #endif
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


// void Puzzles:: alive()
// {
//     puzzle->alive();
// }
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
	fs->sector_count = 4U;
    rc = nvs_mount(fs);
	if (rc) {
        flash_erase(fs->flash_device, NVS_PARTITION_OFFSET, 0x2000);
        rc = nvs_mount(fs);
        if (rc) {
            printk("Flash Init failed, rc=%d\n", rc);
            return 0;
        }
	}
    return 0;
}

void Puzzles:: eraseStorage()
{
    nvs_delete(fs, NVS_PUZZLE_TYPE);
#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#else
    gpio_pin_set_dt(&builtInLed, 1);
#endif
    LOG_INF("Flash erased");
}


void Puzzles:: readInfosFromMemory()
{
//     while(1);




    int rc = 0;
    rc = nvs_read(fs, NVS_PUZZLE_TYPE, &name, PUZZLE_TYPE_NAME_MAX_LEN);
    if(rc > 0)
    {
        // deviceSpecified = true;
        puzzleTypeSelection(name);
    }
}

#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
#else

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
#endif



void puzzleEntryPoint(void *, void *, void *)
{

#ifdef NASER

    #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        char serverIpAddressMqtt[] = "192.168.1.147";
        char serverIpAddressOta[] = "192.168.1.147";
    #else
        char serverIpAddressMqtt[] = "10.42.0.1";
        char serverIpAddressOta[] = "10.42.0.1";
    #endif
#elif defined(POURYA)
    #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        char serverIpAddressMqtt[] = "192.168.1.2";
    #else
        char serverIpAddressMqtt[] = "192.168.1.2";
    #endif
#elif defined(BRAM)
    #if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
        char serverIpAddressMqtt[] = "172.21.10.11";
        char serverIpAddressOta[] = "172.21.10.10";
    #else
        char serverIpAddressMqtt[] = "172.21.10.11";
        char serverIpAddressOta[] = "172.21.10.10";
    #endif
#else
    char serverName[] = "mqtt-1";
    char serverIpAddress[128] = {0};
#endif
    //test();
    // char serverName[] = "test.mosquitto.org";

    struct MqttMsg *msg = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));
    memset(msg, 0, sizeof(struct MqttMsg));
    puzzles = new Puzzles(&fileSystem);
    bool mqtt = false;
#ifdef WATCH_DOG
    puzzles->enableWatchDog();
#endif
    while(!puzzles->deviceSpecified)
    {
        if(!mqtt)
        {
#ifdef NASER
            dhcpClient("not specified");
#elif defined(POURYA)
            dhcpClient("not specified");
#elif defined(BRAM)
            dhcpClient("not specified");

#else
        dnsResolver("not specified", serverName, serverIpAddress);
#endif
            mqttThreadCreate((char*)serverIpAddressMqtt, &puzzleType_topic, 1);
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

    Ota *ota = new Ota(serverIpAddressOta);
#ifdef NASER
            dhcpClient("not specified");
#elif defined(POURYA)
            dhcpClient("not specified");
#elif defined(BRAM)
            dhcpClient("not specified");
#else
            // dnsResolver("not specified", serverName, serverIpAddress);
#endif

    mqttThreadCreate((char*)serverIpAddressMqtt, puzzles->puzzle->getMqttList(), puzzles->puzzle->getMqttCount());
    char command[32] = {0};

    while(1)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            if(puzzles->puzzle->validTopic(msg->topic, command) == 0)
            {
                if(!strcmp("upgrade", command))
                {
                    LOG_INF("upgrading to file: %s", msg->msg);
                    ota->upgrade(msg->msg);
                }
                if(!strcmp("erase", command))
                {
                    if(!strcmp("true", msg->msg))
                    {
                        LOG_INF("eraseing storage");
                        puzzles->eraseStorage();
                        sys_reboot(0);
                    }

                }
            }
            puzzles -> messageHandler(msg);
        }
        // counter++;
        // if(counter > 5)
        // {
        //     puzzles -> alive();
        //     counter = 0;
        // }
        k_msleep(1000);
    }
}

int Puzzles:: writeDeviceName(char *name)
{
    char buf[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    // char buf2[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    strcpy(buf, name);
    return nvs_write(fs, NVS_PUZZLE_TYPE, &buf, PUZZLE_TYPE_NAME_MAX_LEN + 1);
    // nvs_read(fs, 0, &buf2, PUZZLE_TYPE_NAME_MAX_LEN);
}

extern "C" void puzzleThreadCreate()
{
    k_tid_t puzzleTid = k_thread_create(&puzzleThread, puzzleStackArea,
									K_THREAD_STACK_SIZEOF(puzzleStackArea),
									puzzleEntryPoint, NULL, NULL, NULL,
									PUZZLE_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(puzzleTid, "puzzles");
}

int Puzzles:: enableWatchDog()
{

	int err;

	printk("Watchdog sample application\n");

	if (!device_is_ready(wdt)) {
		printk("%s: device not ready.\n", wdt->name);
		// return 0;
	}

	struct wdt_timeout_cfg wdt_config;
    wdt_config.window = { WDT_MIN_WINDOW, WDT_MAX_WINDOW};
		/* Reset SoC when watchdog timer expires. */
    wdt_config.flags = WDT_FLAG_RESET_SOC;

	wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
	if (wdt_channel_id == -ENOTSUP) {
		/* IWDG driver for STM32 doesn't support callback */
		printk("Callback support rejected, continuing anyway\n");
		wdt_config.callback = NULL;
		wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
	}
	if (wdt_channel_id < 0) {
		printk("Watchdog install error\n");
		return 0;
	}

	err = wdt_setup(wdt, WDT_OPT);
	if (err < 0) {
		printk("Watchdog setup error\n");
		return 0;
	}
    wdt_feed(wdt, wdt_channel_id);
}


#include "puzzles.h"
#include "topics.h"
LOG_MODULE_REGISTER(puzzle, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(puzzleStackArea, PUZZLE_STACK_SIZE);

struct k_thread puzzleThread;

static const struct gpio_dt_spec builtInLed = GPIO_DT_SPEC_GET_OR(BUILT_IN_NODE, gpios,
							      {0});


static struct nvs_fs fileSystem;

Puzzle::Puzzle(struct nvs_fs *_fs): fs(_fs)
{
    nvsInit();
    builtIntLedInit();
    // readInfosFromMemory();
    puzzleTypeSelection("config");
    // puzzleTypeSelection("rotating platform");
}



Puzzle *puzzle = nullptr;

void Puzzle:: puzzleTypeSelection(char *type)
{
 
    gpio_pin_set_dt(&builtInLed, 1);

    if(strcmp(type, "servos") == 0)
    {
        puzzleType = SERVOS_PUZZLE;
        servos = new Servos;
        LOG_INF("Puzzle type is Servos");
        deviceSpecified = true;
    }
    else if(strcmp(type, "gate") == 0)
    {
        puzzleType = GATE_PUZZLE;
        gate = new Gate;
        LOG_INF("Puzzle type is Gate.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "config") == 0)
    {
        puzzleType = CONFIG_DEVICE_PUZZLE;
        configDevice = new ConfigDevice;
        LOG_INF("Device is configuring by user.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "numbers guessing") == 0)
    {
        puzzleType = NUMBERS_GUESSING_PUZZLE;
        numbersGuessing = new NumbersGuessing;
        LOG_INF("Puzzle type is numbers guessing.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "nuseen") == 0)
    {
        puzzleType = UNSEEN_PUZZLE;
        unseen = new Unseen;
        LOG_INF("Puzzle type is unseen.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "laboratory") == 0)
    {
        puzzleType = LABORATORY_PUZZLE;
        laboratory = new Laboratory;
        LOG_INF("Puzzle type is laboratory.");
        deviceSpecified = true;
    }
    else if(strcmp(type, "rotating platform") == 0)
    {
        puzzleType = ROTATING_PLATFORM_PUZZLE;
        rotatingPlatform = new RotatingPlatform;
        LOG_INF("Puzzle type is Rotating Platform.");
        deviceSpecified = true;
    }
    else
    {
        deviceSpecified = false;
        LOG_INF("Puzzle type is not recognized.");
        gpio_pin_set_dt(&builtInLed, 0);
    }
}


void Puzzle:: messageHandler(struct MqttMsg *msg)
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
            
            switch (puzzle->puzzleType)
            {
            case GATE_PUZZLE:
                gate -> messageHandler(msg);
                break;
            
            case SERVOS_PUZZLE:
                servos -> messageHandler(msg);
                break;

            case CONFIG_DEVICE_PUZZLE:
                configDevice -> messageHandler(msg);
                break;

            case NUMBERS_GUESSING_PUZZLE:
                numbersGuessing -> messageHandler(msg);
                break;

            case UNSEEN_PUZZLE:
                unseen -> messageHandler(msg);
                break;

            case LABORATORY_PUZZLE:
                laboratory -> messageHandler(msg);

            case ROTATING_PLATFORM_PUZZLE:
                rotatingPlatform -> messageHandler(msg);
            default:
                break;
            }
        }


    }

}

int Puzzle:: nvsInit()
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
}

void Puzzle:: readInfosFromMemory()
{
    int rc = 0;
    char name[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    rc = nvs_read(fs, 0, &name, PUZZLE_TYPE_NAME_MAX_LEN);
    if(rc)
    {
        deviceSpecified = true;
        puzzleTypeSelection(name);
    }
}

int Puzzle:: builtIntLedInit()
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

void puzzleEntryPoint(void *, void *, void *)
{
    struct MqttMsg *msg = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));

    memset(msg, 0, sizeof(struct MqttMsg));
    puzzle = new Puzzle(&fileSystem);
    while(!puzzle->deviceSpecified)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            puzzle -> messageHandler(msg);
            memset(msg, 0, sizeof(struct MqttMsg));
        
        }
        k_msleep(1000);

    }
    
    while(1)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            if(strcmp(msg->topic, PUZZLE_TYPE_TOPIC) == 0)
            {
                puzzle -> writeDeviceName(msg->msg);
                sys_reboot(0);
            }
            else
            {
                puzzle -> messageHandler(msg);
                memset(msg, 0, sizeof(struct MqttMsg));
            }
           
        }
        k_msleep(1000);
    }
}

int Puzzle:: writeDeviceName(char *name)
{
    char buf[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    char buf2[PUZZLE_TYPE_NAME_MAX_LEN] = {0};
    strcpy(buf, name);
    nvs_write(fs, 0, &buf, PUZZLE_TYPE_NAME_MAX_LEN + 1);
    nvs_read(fs, 0, &buf2, PUZZLE_TYPE_NAME_MAX_LEN);
    int a = 3;
    return a;
}

extern "C" void puzzleThreadCreate()
{
    k_tid_t puzzleTid = k_thread_create(&puzzleThread, puzzleStackArea,
									K_THREAD_STACK_SIZEOF(puzzleStackArea),
									puzzleEntryPoint, NULL, NULL, NULL,
									PUZZLE_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(puzzleTid, "puzzle");
}



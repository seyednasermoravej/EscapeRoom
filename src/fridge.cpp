#include "fridge.h"
#include "lvgl_mem.h"
LOG_MODULE_REGISTER(fridge, LOG_LEVEL_DBG);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(fridge_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

#define STRIP_NODE		DT_NODELABEL(ws2812)

#if DT_NODE_HAS_PROP(DT_NODELABEL(ws2812), chain_length)
static const uint8_t wsChainLength = DT_PROP(DT_NODELABEL(ws2812), chain_length);
#else
#error Unable to determine length of LED strip
#endif

static const struct device *const pio1_dev = DEVICE_DT_GET(DT_NODELABEL(pio1));



#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/sys_heap.h>

#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include <ff.h>


#define DISK_DRIVE_NAME "SD"
#define DISK_MOUNT_PT "/"DISK_DRIVE_NAME":"
#define FS_RET_OK FR_OK


static Fridge *instance = nullptr;

static FATFS fat_fs;

/* mounting info */
static struct fs_mount_t mp = {
	.type = FS_FATFS,
	.fs_data = &fat_fs,
};


static const char *disk_mount_pt = DISK_MOUNT_PT;



#define DISPLAY_NODE          DT_CHOSEN(zephyr_display)

static lv_display_t *display;
static struct lvgl_disp_data disp_data = {0};


extern int lvgl_init();
static uint32_t count;
Fridge:: Fridge(const char *room, const char *type): Puzzle(room, type)
{
    device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(display_mipi_dbi)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(ili9488_buydisplay_3_5_tft_touch_arduino)));
    

    int err = 0;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		// return 0;
	}
    lvgl_init();
    device_init(DEVICE_DT_GET(DT_NODELABEL(sdhc0)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(mmc)));

	mp.mnt_point = disk_mount_pt;
	int res = fs_mount(&mp);
	if (res == FS_RET_OK) {
		printk("Disk mounted.\n");
		}
	else {
		printk("Error mounting disk.\n");
	}


    device_init(pio1_dev);
    ledStrip = new LedStrip(DEVICE_DT_GET(STRIP_NODE), wsChainLength);

    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(allRelays); i++){
        if (!device_is_ready(allRelays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&allRelays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    // keypad = new Keypad43(mqttCommand);
    device_init(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix43)));
    INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(keypad43)), keypadHandlerWrapper, (void*)this);

    instance = this;

    creatingMqttList();
}


void Fridge:: keypadHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY) 
    {
        if(val->value)
        {
            if((passwordSet) && lang)
            {
                struct MqttMsg msg = {0};
                if((val->code < INPUT_KEY_0))
                {
                    sprintf(msg.topic, "%skeypad%d", mqttCommand, val->code - INPUT_KEY_1 + 1);
                    puzzleSolver(val->code - INPUT_KEY_1 + '0');
                }
                if(val->code == INPUT_KEY_0)
                {
                    sprintf(msg.topic, "%skeypad0", mqttCommand);
                    puzzleSolver('0');
                }
                if(val->code == INPUT_KEY_ENTER)
                {
                    sprintf(msg.topic, "%skeypadEnter", mqttCommand);
                }
                if(val->code == INPUT_KEY_ESC)
                {
                    sprintf(msg.topic, "%skeypadEsc", mqttCommand);
                }
                sprintf(msg.msg, "true");
                LOG_INF("%s",msg.topic);
                k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
            }
        }
    }
}

void Fridge:: keypadHandlerWrapper(struct input_event *val, void *userData)
{

    instance->keypadHandler(val);
    // ((HeartBox *)userData)->keypadHandler(val);
}


void Fridge:: puzzleSolver(char input)
{
    // if((passwordSet) && lang)
    // {
    //     static uint8_t pos = 0;
    //     guess[PASS_LEN_FRIDGE + 1] = '\0';
    //     if(input == password[pos])
    //     {
    //         guess[pos] = input;
    //         display8->displayStr(guess);
    //         pos++;
    //         if(pos == PUZZLE_DISPLAY_LEN)
    //         {
    //             pos = 0;
    //             struct MqttMsg msg = {0};
    //             sprintf(msg.topic, "%ssolved", mqttCommand);
    //             sprintf(msg.msg, "true");
    //             k_msgq_put(&msqSendToMQTT, &msg, K_FOREVER);
    //             LOG_INF("topic: %s, msg: %s", msg.topic, msg.msg);
    //             passwordSet = false;
    //             lang = NONE;

    //         }

    //     }
    //     {
    //         pos = 0;
    //         memset(guess, 32, PUZZLE_DISPLAY_LEN);//32 = char space 
    //         guess[PUZZLE_DISPLAY_LEN] = '\0';


    //         memset(guess, 56, PUZZLE_DISPLAY_LEN);//56 = char 8
    //         display8->displayStr(guess);
    //         k_msleep(200);

    //         memset(guess, 32, PUZZLE_DISPLAY_LEN);
    //         display8->displayStr(guess);
    //         k_msleep(200);

    //         memset(guess, 56, PUZZLE_DISPLAY_LEN);//56 = char 8
    //         display8->displayStr(guess);
    //         k_msleep(200);

    //         memset(guess, 32, PUZZLE_DISPLAY_LEN);
    //         display8->displayStr(guess);

    //         struct MqttMsg msg = {0};
    //         sprintf(msg.topic, "%ssolved", mqttCommand);
    //         sprintf(msg.msg, "false");
    //         k_msgq_put(&msqSendToMQTT, &msg, K_FOREVER);
    //         LOG_INF("topic: %s, msg: %s", msg.topic, msg.msg);
    //     }

    // }
}

void Fridge:: lcdInit(LangSelection _lang)
{
    lang = _lang;
    lv_obj_t * background = lv_obj_create(NULL);
	lv_scr_load(background);
	lv_obj_set_style_bg_color(background,lv_color_black() , 0);
//////////////
	LV_IMAGE_DECLARE(star_icon);

	lv_obj_t * icon_Star_1 = lv_image_create(lv_scr_act());
	lv_image_set_src(icon_Star_1, &star_icon);
	lv_obj_align(icon_Star_1, LV_ALIGN_BOTTOM_LEFT,10,-60);

	lv_obj_t * icon_Star_2 = lv_image_create(lv_scr_act());
	lv_image_set_src(icon_Star_2, &star_icon);
	lv_obj_align(icon_Star_2, LV_ALIGN_BOTTOM_LEFT,90,-60);

	lv_obj_t * icon_Star_3 = lv_image_create(lv_scr_act());
	lv_image_set_src(icon_Star_3, &star_icon);
	lv_obj_align(icon_Star_3, LV_ALIGN_BOTTOM_LEFT,170,-60);

	lv_obj_t * icon_Star_4 = lv_image_create(lv_scr_act());
	lv_image_set_src(icon_Star_4, &star_icon);
	lv_obj_align(icon_Star_4, LV_ALIGN_BOTTOM_LEFT,250,-60);

	lv_obj_t * icon_Star_5 = lv_image_create(lv_scr_act());
	lv_image_set_src(icon_Star_5, &star_icon);
	lv_obj_align(icon_Star_5, LV_ALIGN_BOTTOM_LEFT,330,-60);

	lv_obj_t * icon_Star_6 = lv_image_create(lv_scr_act());
	lv_image_set_src(icon_Star_6, &star_icon);
	lv_obj_align(icon_Star_6, LV_ALIGN_BOTTOM_LEFT,410,-60);

////////////

    for(int i = 0; i < 6; i++)
    {

    }
	lv_obj_t * label_digit_1 = lv_label_create(lv_scr_act());
	lv_label_set_text(label_digit_1,"");
	lv_obj_set_style_text_color(label_digit_1,lv_color_hex(0xFF0000),0);
	lv_obj_set_width(label_digit_1,50);
	lv_obj_align(label_digit_1, LV_ALIGN_BOTTOM_LEFT,23,-60);

	lv_obj_t * label_digit_2 = lv_label_create(lv_scr_act());
	lv_label_set_text(label_digit_2,"");
	lv_obj_set_style_text_color(label_digit_2,lv_color_hex(0xFF0000),0);
	lv_obj_set_width(label_digit_2,50);
	lv_obj_align(label_digit_2, LV_ALIGN_BOTTOM_LEFT,103,-60);

	lv_obj_t * label_digit_3 = lv_label_create(lv_scr_act());
	lv_label_set_text(label_digit_3,"");
	lv_obj_set_style_text_color(label_digit_3,lv_color_hex(0xFF0000),0);
	lv_obj_set_width(label_digit_3,50);
	lv_obj_align(label_digit_3, LV_ALIGN_BOTTOM_LEFT,183,-60);

	lv_obj_t * label_digit_4 = lv_label_create(lv_scr_act());
	lv_label_set_text(label_digit_4,"");
	lv_obj_set_style_text_color(label_digit_4,lv_color_hex(0xFF0000),0);
	lv_obj_set_width(label_digit_4,50);
	lv_obj_align(label_digit_4, LV_ALIGN_BOTTOM_LEFT,263,-60);

	lv_obj_t * label_digit_5 = lv_label_create(lv_scr_act());
	lv_label_set_text(label_digit_5,"");
	lv_obj_set_style_text_color(label_digit_5,lv_color_hex(0xFF0000),0);
	lv_obj_set_width(label_digit_5,50);
	lv_obj_align(label_digit_5, LV_ALIGN_BOTTOM_LEFT,353,-60);

	lv_obj_t * label_digit_6 = lv_label_create(lv_scr_act());
	lv_label_set_text(label_digit_6,"");
	lv_obj_set_style_text_color(label_digit_6,lv_color_hex(0xFF0000),0);
	lv_obj_set_width(label_digit_6,50);
	lv_obj_align(label_digit_6, LV_ALIGN_BOTTOM_LEFT,425,-60);

    //////
    lv_obj_t * screen_text = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(screen_text, &lv_font_montserrat_30, 0);
	lv_obj_set_style_text_color(screen_text,lv_color_white(),0);

    switch (lang)
    {
    case ENGLISH:
        lv_label_set_text(screen_text,"you cannot see it");
        lv_obj_set_width(screen_text,300);
        break;
    case DUTCH:
        lv_label_set_text(screen_text,"Je kunt het niet zien");
        lv_obj_set_width(screen_text,320);
        break;
    case FRENCH:
        lv_label_set_text(screen_text,"Tu ne peux pas le voir ");
        lv_obj_set_width(screen_text,340);
        break;
    default:
        break;
    }

    lv_obj_align(screen_text, LV_ALIGN_CENTER,0,-40);
}
void Fridge:: creatingMqttList()
{
    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[i + systemTopicsNo] = *createMqttTopic(topic);
    }
    for(uint8_t i = 0; i < wsChainLength; i++) /// chain length in overlay
    {
        sprintf(topic, "%sws2811_%d", mqttCommand, i + 1);
        mqttList[i + systemTopicsNo + ARRAY_SIZE(allRelays)] = *createMqttTopic(topic);
    }

    // for(uint8_t i = 0; i < numOfDisplays; i++)
    // {
    //     sprintf(topic, "%sdisplay%d", mqttCommand, i + 1);
    //     mqttList[wsChainLength + ARRAY_SIZE(allRelays) + i] = *createMqttTopic(topic);
    // }
    sprintf(topic, "%slanguage", mqttCommand);
    mqttList[systemTopicsNo + wsChainLength + ARRAY_SIZE(allRelays)] = *createMqttTopic(topic);

    sprintf(topic, "%spassword", mqttCommand);
    mqttList[systemTopicsNo + wsChainLength + ARRAY_SIZE(allRelays) + 1] = *createMqttTopic(topic);

    mqttCount = systemTopicsNo + wsChainLength + ARRAY_SIZE(allRelays) + 2;
}


void Fridge:: messageHandler(struct MqttMsg *msg)
{
    int rc;
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strcmp(command, "language") == 0)
        {

            // lv_obj_t * my_screen = lv_image_create(lv_scr_act());
            // char address[64] = {0};
            // sprintf(address, "%s/%s%s", DISK_MOUNT_PT, msg->msg, ".bin");
            // LOG_INF("printing %s", address);
            // lv_image_set_src(my_screen, address);
            // lv_obj_align(my_screen, LV_ALIGN_CENTER, 0, 0);
            // lv_obj_set_size(my_screen,320,480);

            // lv_task_handler();
            // display_blanking_off(display_dev);
            // if(strcmp(msg->msg, "english") == 0)
            // {
            //     lang = ENGLISH;
            //     lcdInit(lang);

            // }
            // else if(strcmp(msg->msg, "dutch") == 0)
            // {
            //     lang = DUTCH;
            //     lcdInit(lang);
            // }
            // else if(strcmp(msg->msg, "french") == 0)
            // {
            //     lang = FRENCH;
            //     lcdInit(lang);
            // }
            // else
            // {
            //     LOG_INF("Language is not valid");
            //     lang = NONE;
            // }

        }
        else if(strcmp(command, "password") == 0)
        {
            if(strlen(msg->msg) == 6)
            {
                strcpy(password, msg->msg);
                passwordSet = true;
            }
            else
            {
                passwordSet = false;
                LOG_INF("password len not valid");
            }

        }
        else if(strstr(command, "relay") != NULL)
        {
            char field[] = "relay";
            int commandIdx = peripheralIdx(field, command);
            uint8_t relayIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
            {
                if(relayIdx == 1)
                {
                    relayOperation(msg->msg, &allRelays[relayIdx], true);
                }
                else
                {
                    relayOperation(msg->msg, &allRelays[0], false);
                }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else if(strstr(command, "ws2811_") != NULL)
        {
            char field[] = "ws2811_";
            int commandIdx = peripheralIdx(field, command);
            uint8_t ws2811Idx = commandIdx - 1;
            if((commandIdx > 0 ) && (ws2811Idx < 8))
            {
                 //LOG_ERR("a valid index");
                struct led_rgb color_leds = retrieveColors(msg->msg);
                LOG_INF("r: %u g: %u b: %u", color_leds.r, color_leds.g, color_leds.b);

                rc = ledStrip->update(color_leds, ws2811Idx);
                if (rc) {
				    LOG_ERR("couldn't update strip: %d", rc);
			    }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }
        }
    }
    else
        LOG_INF("the command is not valid");
}


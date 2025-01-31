#include "tft_display.h"

LOG_MODULE_REGISTER(tft_display, LOG_LEVEL_DBG);
Display_tft::Display_tft(const struct device *_display_dev): display_dev(_display_dev)
{
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		printk("Device not ready, aborting test");
	}

    background = lv_obj_create(NULL);
	lv_scr_load(background);
	lv_obj_set_style_bg_color(background,lv_color_black() , 0);

    LV_IMAGE_DECLARE(star_icon);
    for(int i=0 ; i<6 ; i++)
	{
		icon_Stars[i]=lv_image_create(lv_scr_act());
		lv_image_set_src(icon_Stars[i], &star_icon);
		lv_obj_align(icon_Stars[i], LV_ALIGN_BOTTOM_LEFT,icon_Stars_pos[i],-60);
	}

    for(int i=0 ; i<6 ; i++)
	{
		label_digits[i]=lv_label_create(lv_scr_act());
		lv_label_set_text(label_digits[i],"");
		lv_obj_set_style_text_color(label_digits[i],lv_color_hex(0xFF0000),0);
		lv_obj_set_width(label_digits[i],48);
		lv_obj_align(label_digits[i], LV_ALIGN_BOTTOM_LEFT,label_digits_pos[i],-60);
	}
    screen_text = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(screen_text, &lv_font_montserrat_30, 0);
	lv_obj_set_style_text_color(screen_text,lv_color_white(),0);
    lv_label_set_text(screen_text,"");
    lv_task_handler();
	display_blanking_off(display_dev);
}


void Display_tft:: displayClear()
{
    // for(uint8_t i=0 ; i<6 ; i++)
    // {
    // lv_label_set_text(label_digits[i],"");
    // }
    displayStr("      ");
}
bool Display_tft::langSelect(char *language)
{

    if(strcmp(language, "english") == 0)
    {
        lv_label_set_text(screen_text,"you cannot see it");
        lv_obj_set_width(screen_text,300);
    }
    else if(strcmp(language, "dutch") == 0)
    {
        lv_label_set_text(screen_text,"Je kunt het niet zien");
        lv_obj_set_width(screen_text,320);
    }
    else if(strcmp(language, "french") == 0)
    {
        lv_label_set_text(screen_text,"Tu ne peux pas le voir ");
        lv_obj_set_width(screen_text,340);
    }
    else
    {
        LOG_INF("Language is not supported");
        return false;
    }

    lv_obj_align(screen_text, LV_ALIGN_TOP_MID,0,70);
    lv_task_handler();
    return true;
}

void Display_tft::displayStr(char *str)
{
    LOG_DBG("str len: %d", strlen(str));
    char _str[2]="";
    // char _str;
    for(uint8_t i=0 ; i<strlen(str) ; i++)
	{
        if(i==6)
        {
            break;
        }
        else
        {
            // _str=str[i];
            _str[0]=str[i];
            lv_label_set_text(label_digits[i],_str);
        }
	}
    LOG_DBG("before tasks handle");
    lv_task_handler();
}
//  extern "C" void tft_code(void)
//  {
//     Display_tft * my_screen = new Display_tft(DEVICE_DT_GET(DT_CHOSEN(zephyr_display)));
//     my_screen->langSelect(FRENCH);
//     while (1) {
// 		lv_task_handler();
// 		k_sleep(K_MSEC(5000));
//         my_screen->displayPass(my_screen->displaying_pass);
//     }
    
//  }

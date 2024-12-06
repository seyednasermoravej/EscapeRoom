#include "tftLcd.h"

LOG_MODULE_REGISTER(tftLcd, LOG_LEVEL_INF);

TftLcd:: TftLcd(const struct device *dev): displayDev(dev)
{
    if(!displayDev)
    {
       LOG_ERR("Display device not found");
       return;
    }
    // lv_init();
    // lv_disp_draw_buf_t draw_buf;
    // static lv_color_t buf1[LV_HOR_RES_MAX * 10];
    // lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LV_HOR_RES_MAX * 10);

    // lv_disp_drv_t disp_drv;
    // lv_disp_drv_init(&disp_drv);
    // disp_drv.draw_buf = &draw_buf;
    // disp_drv.flush_cb = your_flush_cb; // Implement your flush callback
    // lv_disp_drv_register(&disp_drv);

    // lv_obj_t *img = lv_img_create(lv_scr_act());
    // lv_img_set_src(img, &your_image); // your_image is the converted image array
    // lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    // while (1) {
    //     lv_task_handler();
    //     k_sleep(K_MSEC(10));
    // }

//     LV_IMG_DECLARE(rainbow);
// 	lv_obj_t * my_rainbow = lv_img_create(lv_scr_act());
// 	lv_img_set_src(my_rainbow, &rainbow);
// 	lv_obj_align(my_rainbow, LV_ALIGN_CENTER, 0, 0);
// 	lv_obj_set_size(my_rainbow,240,320);
// /////////////////
// 	lv_task_handler();
// 	display_blanking_off(displayDev);

// 	while (1) {
// 		lv_task_handler();
// 		k_sleep(K_MSEC(10));
// 	}
}
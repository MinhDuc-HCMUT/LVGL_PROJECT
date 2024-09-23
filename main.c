#include "lvgl/lvgl.h"
//#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
//#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "lv_demos/lv_demo.h"

#define DISP_BUF_SIZE (128 * 64)

// void lv_show_img(lv_obj_t * img, const lv_img_dsc_t img_dat) {
//     lv_obj_clean(img);
//     lv_img_set_src(img, &img_dat);
//     lv_obj_center(img);
// }

void fbdev_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 128;
    disp_drv.ver_res    = 64;
    lv_disp_drv_register(&disp_drv);

    // Create Calendar
    lv_obj_t *calendar = lv_calendar_create(lv_scr_act()); 
    lv_obj_set_size(calendar, 128, 64);                    
    lv_obj_center(calendar);  
    
    // Create a style with smaller font
    static lv_style_t style_small_font;
    lv_style_init(&style_small_font);
    lv_style_set_text_font(&style_small_font, &lv_font_montserrat_10); // Use Montserrat 10 (small font)

    // Apply the style to the calendar
    lv_obj_add_style(calendar, &style_small_font, 0);                             

    // Set today's date
    lv_calendar_set_today_date(calendar, 2024, 9, 18); // Year, Month, Day

    // Set the displayed date
     lv_calendar_set_showed_date(calendar, 2024, 9); // Year, Month, Day        

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

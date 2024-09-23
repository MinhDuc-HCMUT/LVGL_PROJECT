### 1. Configure the FB device in Linux Board (dts,config)
### 2. Fetch the resources:
*Create a folder in the root directory to store the official source code:*
```bash
luckfox@luckfox:~$ mkdir lvgl
luckfox@luckfox:~$ cd lvgl
```
*Fetch the resources:*
```bash
git clone -b v8.1.0 https://github.com/lvgl/lvgl.git
git clone -b v8.1.0 https://github.com/lvgl/lv_drivers.git
git clone -b v8.1.0 https://github.com/lvgl/lv_demos.git
git clone --branch release/v8.2 --single-branch https://github.com/lvgl/lv_port_linux_frame_buffer.git
```
*Create project (example **fbv_loadbitmap**)*
```bash
luckfox@luckfox:~$ mkdir lvgl_project
luckfox@luckfox:~$ cd lvgl_project/
luckfox@luckfox:~/lvgl_project$ mkdir fbv_loadbitmap
luckfox@luckfox:~/lvgl_project$ cd fbv_loadbitmap/
```
### 3.Copy file resources:
* *Copy the **lvgl** and **lv_drivers** directories from the **root** directory*
* *Copy **main.c** and **Makefile** from **lv_port_linux_frame_buffer***
* *Copy **lv_conf_template.h** from **lvgl** and rename it to **lv_conf.h***
* *Copy **lv_drv_conf_template.h** from **lv_drivers** and rename it to **lv_drv_conf.h***
```bash
cp -r ~/lvgl/lvgl ./
cp -r ~/lvgl/lv_drivers ./
cp ~/lvgl/lvgl/lv_conf_template.h ./lv_conf.h
cp ~/lvgl/lv_drivers/lv_drv_conf_template.h  ./lv_drv_conf.h
cp ~/lvgl/lv_port_linux_frame_buffer/main.c ./
cp ~/lvgl/lv_port_linux_frame_buffer/Makefile ./
```
*Check the **fbv_loadbitmap** project directory:*
```bash
luckfox@luckfox:~/lvgl_project/fbv_loadbitmap$ ls -l
total 64
-rw-rw-r--  1 luckfox luckfox 29023 12月 27 19:14 lv_conf.h
drwxrwxr-x 12 luckfox luckfox  4096 12月 27 19:14 lv_drivers
-rw-rw-r--  1 luckfox luckfox 15184 12月 27 19:14 lv_drv_conf.h
drwxrwxr-x 12 luckfox luckfox  4096 12月 27 19:14 lvgl
-rw-rw-r--  1 luckfox luckfox  2350 12月 27 19:14 main.c
-rw-rw-r--  1 luckfox luckfox  2321 12月 27 19:14 Makefile
```
### 4.Modify 
**lv_conf.h**
1. Enable
Change `#if 0` to `#if 1` at the beginning
```c
/* clang-format off */
#if 1 /*Set it to "1" to enable content*/
```
2. Allocate VRAM
Enable `LV_MEM_CUSTOM` and choose to allocate VRAM manually
```c
/*1: use custom malloc/free, 0: use the built-in `lv_mem_alloc()` and `lv_mem_free()`*/
#define LV_MEM_CUSTOM 1
```
3. Refresh time
Change the original 30ms to 10ms
```c
/*Default display refresh period. LVG will redraw changed areas with this period time*/
#define LV_DISP_DEF_REFR_PERIOD 10      /*[ms]*/
/*Input device read period in milliseconds*/
#define LV_INDEV_DEF_READ_PERIOD 10     /*[ms]*/
```
4. TICK Configuration
Enable `LV_TICK_CUSTOM` and choose to customize the Tick timer configuration function in the application

Original content:
```c
#define LV_TICK_CUSTOM 0
#if LV_TICK_CUSTOM
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"         /*Header for the system time function*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())    /*Expression evaluating to current system time in ms*/
#endif   /*LV_TICK_CUSTOM*/
```

Change to:
```c
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE <stdint.h>         /*Header for the system time function*/
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (custom_tick_get())    /*Expression evaluating to current system time in ms*/
#endif   /*LV_TICK_CUSTOM*/
```
**lv_drv_conf.h**

1. Enable

Change `#if 0` to `#if 1` at the beginning
```c
/* clang-format off */
#if 1 /*Set it to "1" to enable the content*/
```
2. Support devices

Enable USE_FBDEV to support Framebuffer devices
```c
/*-----------------------------------------
 *  Linux frame buffer device (/dev/fbx)
 *-----------------------------------------*/
#ifndef USE_FBDEV
#  define USE_FBDEV           1
#endif

#if USE_FBDEV
#  define FBDEV_PATH          "/dev/fb0"
#endif
```
**Makefile**

Modify <SDK Directory\> in the Makefile to your SDK path, example: `CC = /home/ducbui/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc`

**main.c**

Modify the resolution to match the screen you are using
```c
#define DISP_BUF_SIZE (128 * 64)
...
disp_drv.hor_res    = 128;
disp_drv.ver_res    = 64;
```


#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include "./common.h"
#include "./font_manager.h"

typedef struct DispInfo {
  int x_res;
  int y_res;
  int bpp;
  unsigned char *fb_base;
  int screen_size;
  int pixel_width; // 像素宽度，单位字节，bpp为32，宽度就为4字节
  int line_width;
} DispInfo, *DispInfoPtr;

int flush_region(RegionPtr region, unsigned char *buffer);
void put_pixel(int x, int y, unsigned int color);
int display_init(void);
int display_exit(void);
void get_disp_info(DispInfoPtr disp_info_ptr);
void draw_bitmap(FontBitMapPtr bitmap, unsigned int color);
void draw_region(Region region, int color);
void draw_text_in_region_central(char *name, Region region, int color);

#endif
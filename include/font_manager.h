#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "./common.h"

typedef struct FontBitMap {
  Region region;
  int cur_origin_x;
  int cur_origin_y;
  int next_origin_x;
  int next_origin_y;
  unsigned char *font_buffer;
} FontBitMap, *FontBitMapPtr;

typedef struct FontOpr {
  char *name;
  int (*font_init)(char *font_name);
  void (*set_font_size)(int size);
  int (*get_string_region_car)(char *str,
                               RegionCartesianPtr region_cartesian_p);
  int (*get_font_bit_map)(unsigned int char_code,
                          FontBitMapPtr font_bit_map_ptr);
  struct FontOpr *next;
} FontOpr, *FontOprPtr;

void register_fonts(FontOprPtr ptr);
void fonts_register(void);
int select_and_init_font(char *fontname, char *filepathname);
void set_font_size(int size);
int get_font_bitmap(unsigned int code, FontBitMapPtr font_bit_map_ptr);
int get_string_region_car(char *str, RegionCartesianPtr region_cartesian_p);

#endif
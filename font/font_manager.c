#include "../include/font_manager.h"
#include <stdlib.h>
#include <string.h>

static FontOprPtr font_opr_ptr = NULL;
static FontOprPtr cur_font_opr_ptr = NULL;

void fonts_register(void) {
  extern void register_freetype_font(void);
  register_freetype_font();
}

int select_and_init_font(char *fontname, char *filepathname) {
  FontOprPtr ptr = font_opr_ptr;
  while (ptr) {
    if (strcmp(ptr->name, fontname) == 0) {
      break;
    }
    ptr = ptr->next;
  }

  if (!ptr) {
    return -1;
  }

  cur_font_opr_ptr = ptr;
  return ptr->font_init(filepathname);
}

void set_font_size(int size) {
 cur_font_opr_ptr->set_font_size(size);
}

int get_font_bitmap(unsigned int code, FontBitMapPtr font_bit_map_ptr) {
  return cur_font_opr_ptr->get_font_bit_map(code, font_bit_map_ptr);
}

void register_fonts(FontOprPtr ptr) {
  ptr->next = font_opr_ptr;
  font_opr_ptr = ptr;
}

int get_string_region_car(char *str, RegionCartesianPtr region_cartesian_p) {
  return cur_font_opr_ptr->get_string_region_car(str, region_cartesian_p);
}
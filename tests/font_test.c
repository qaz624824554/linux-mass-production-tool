#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// <filepathname> <str> <x> <y> <fontsize>
int main(int argc, char **argv) {
  if (argc != 6) {
    printf("Usage need 5 argument\n");
    return 1;
  }

  int error;
  int lcd_x = strtol(argv[3], NULL, 0);
  int lcd_y = strtol(argv[4], NULL, 0);
  int font_size = strtol(argv[5], NULL, 0);

  FontBitMap font_bit_map;
  char *str = argv[2];

  display_init();

  DispInfo disp_info;

  get_disp_info(&disp_info);

  fonts_register();

  error = select_and_init_font("freetype", argv[1]);
  if (error) {
    printf("select_and_init_font error\n");
    return -1;
  }

  set_font_size(font_size);

  for (int i = 0; i < strlen(str); i++) {
    font_bit_map.cur_origin_x = lcd_x;
    font_bit_map.cur_origin_y = lcd_y;

    if ((get_font_bitmap(str[i], &font_bit_map))) {
      printf("get_font_bitmap error\n");
      return -1;
    }

    draw_bitmap(&font_bit_map, 0xff0);

    lcd_x = font_bit_map.next_origin_x;
    lcd_y = font_bit_map.next_origin_y;
  }

  display_exit();

  return 0;
}

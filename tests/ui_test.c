#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include "../include/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// <filepathname> <str> <x> <y> <fontsize>
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage need 5 argument\n");
    return 1;
  }

  int error;

  display_init();

  DispInfo disp_info;

  get_disp_info(&disp_info);

  fonts_register();

  error = select_and_init_font("freetype", argv[1]);
  if (error) {
    printf("select_and_init_font error\n");
    return -1;
  }

  Region region;
  region.left_top_x = 200;
  region.left_top_y = 200;
  region.width = 300;
  region.height = 100;

  Button button;

  init_button(&button, "test", &region, NULL, NULL);
  button.on_draw(&button, &disp_info);

  while (1) {
    button.on_press(&button, &disp_info, NULL);
    sleep(2);
  }

  display_exit();

  return 0;
}

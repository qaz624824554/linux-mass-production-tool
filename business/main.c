#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include "../include/input_manager.h"
#include "../include/page_manager.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("need 2 arguments\n");
    return -1;
  }

  DispInfo disp_info;

  /*
    初始化显示系统
  */
  display_init();

  get_disp_info(&disp_info);

  printf("初始化显示系统完成\n");

  /*
    初始化输入系统
  */

  input_init();

  input_device_init();

  printf("初始化输入系统完成\n");

  /*
    初始化文字系统
  */

  fonts_register();

  select_and_init_font("freetype", argv[1]);

  printf("初始化文字系统完成\n");

  /*
    初始化页面系统
  */

  pages_register();

  printf("初始化页面系统完成\n");

  page("main")->run(NULL);

  display_exit();

  return 0;
}

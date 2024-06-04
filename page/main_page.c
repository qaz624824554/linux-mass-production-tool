#include <fcntl.h>
#include <linux/fb.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/config.h"
#include "../include/page_manager.h"
#include "../include/ui.h"

#define X_GAP 5
#define Y_GAP 5
static Button buttons[MAX_ITEM_COUNT];
static int item_count;

static int get_font_size_for_all_button(void) {

  int i;
  int max_len = -1;
  int max_index = 0;
  int len;
  RegionCartesian region_cartesian;
  float k, kx, ky;

  /* 1. 找出name最长的Button */
  for (i = 0; i < item_count; i++) {
    len = strlen(buttons[i].name);
    if (len > max_len) {
      max_len = len;
      max_index = i;
    }
  }

  /* 2. 以font_size =100, 算出它的外框 */
  set_font_size(100);
  get_string_region_car(buttons[max_index].name, &region_cartesian);

  /* 3. 把文字的外框缩放为Button的外框 */
  kx = (float)buttons[max_index].region.width / region_cartesian.width;
  ky = (float)buttons[max_index].region.height / region_cartesian.height;
  if (kx < ky)
    k = kx;
  else
    k = ky;

  /* 4. 反算出font size, 只取0.80, 避免文字过于接近边界 */
  return k * 100 * 0.8;
}

static int main_page_on_press(ButtonPtr button_p, DispInfoPtr disp_info_p,
                              InputEventPtr input_event_p) {
  int bg_color = BUTTON_BG_COLOR;
  char *str = button_p->name;
  int command_status_index = 0;
  char command[1000];
  char *command_status[3] = {"err", "ok", "percent"};
  ItemConfigPtr item_config_p = NULL;

  // 触摸屏事件
  if (input_event_p->type == INPUT_TYPE_TOUCH) {
    if (get_item_by_name(button_p->name)->is_enabled_touch == 0) {
      printf("该按钮不支持触摸事件\n");
      return -1;
    }
    button_p->status = !button_p->status;

    if (button_p->status) {
      bg_color = BUTTON_BG_PRESSED_COLOR;
      command_status_index = 1;
    }

  }

  // 网络事件，根据传入的字符串修改颜色
  else if (input_event_p->type == INPUT_TYPE_NET) {
    char name[100];
    char status[20];
    sscanf(input_event_p->str, "%s %s", name, status);
    if (strcmp(status, "ok") == 0) {
      bg_color = BUTTON_BG_PRESSED_COLOR;
      command_status_index = 1;
    } else if (strcmp(status, "err") == 0) {
      bg_color = BUTTON_BG_COLOR;
      command_status_index = 0;
    } else if (status[0] >= '0' && status[0] <= '9') {
      bg_color = BUTTON_PERCENT_COLOR;
      str = status;
      command_status_index = 2;
    } else {
      return -1;
    }
  } else {
    return -1;
  }

  draw_region(button_p->region, bg_color);

  draw_text_in_region_central(str, button_p->region, BUTTON_TEXT_COLOR);

  /* 执行command */
  item_config_p = get_item_by_name(button_p->name);
  if (item_config_p->command[0] != '\0') {
    sprintf(command, "./%s %s", item_config_p->command,
            command_status[command_status_index]);
    system(command);
  }

  return 0;
}

// 生成页面按钮
static int generate_buttons(void) {

  // 算出单个按钮的width/height
  DispInfo disp_info;
  int width, height;
  int x_res, y_res;
  int n_per_line;
  int rows;
  item_count = get_item_count();
  get_disp_info(&disp_info);

  x_res = disp_info.x_res;
  y_res = disp_info.y_res;

  width = sqrt(1.0 / 0.618 * x_res * y_res / item_count);
  n_per_line = x_res / width + 1;
  width = x_res / n_per_line;
  height = width * 0.618;

  // 居中显示计算
  int start_x, start_y;
  start_x = (x_res - width * n_per_line) / 2;
  rows = item_count / n_per_line;
  if (rows * n_per_line < item_count)
    rows++;

  start_y = (y_res - rows * height) / 2;

  // 计算每个按钮的region
  int pre_start_x, pre_start_y;
  ButtonPtr button_ptr;
  int i = 0;
  for (int row = 0; (row < rows) && (i < item_count); row++) {
    pre_start_y = start_y + row * height;
    pre_start_x = start_x - width;
    for (int col = 0; (col < n_per_line) && (i < item_count); col++) {
      button_ptr = &buttons[i];
      button_ptr->region.left_top_x = pre_start_x + width;
      button_ptr->region.left_top_y = pre_start_y;
      button_ptr->region.width = width - X_GAP;
      button_ptr->region.height = height - Y_GAP;
      pre_start_x = button_ptr->region.left_top_x;

      init_button(button_ptr, get_item_by_index(i)->name, NULL, NULL,
                  main_page_on_press);
      i++;
    }
  }

  int font_size = get_font_size_for_all_button();
  printf("font_size = %d\n", font_size);

  // 绘制每个按钮
  for (i = 0; i < item_count; i++) {
    buttons[i].font_size = font_size;
    buttons[i].on_draw(&buttons[i], &disp_info);
  }

  printf("页面按钮绘制成功\n");
  return 0;
}

// 判断触发的x，y是否在按钮内
static int is_touch_region(int x, int y, RegionPtr region_p) {
  if ((x > region_p->left_top_x &&
       x < region_p->left_top_x + region_p->width) &&
      (y > region_p->left_top_y && y < region_p->left_top_y + region_p->height))
    return 1;
  return 0;
}

// 根据输入事件获取按钮
static int get_button_by_input_event(InputEventPtr input_event_p,
                                     ButtonPtr *button_pp) {
  // 打印参数
  printf("type: %d, x: %d, y: %d, str: %s\n", input_event_p->type,
         input_event_p->x, input_event_p->y, input_event_p->str);

  if (input_event_p->type == INPUT_TYPE_TOUCH) {
    for (int i = 0; i < item_count; i++) {
      if (is_touch_region(input_event_p->x, input_event_p->y,
                          &buttons[i].region)) {
        // 打印按钮值
        printf("触发按钮：%s\n", buttons[i].name);
        *button_pp = &buttons[i];
        return 0;
      }
    }
  } else if (input_event_p->type == INPUT_TYPE_NET) {
    char name[100];
    sscanf(input_event_p->str, "%s", name);
    for (int i = 0; i < item_count; i++) {
      if ((strcmp(name, buttons[i].name) == 0)) {
        *button_pp = &buttons[i];
        return 0;
      }
    }
  }

  return -1;
}

static void run(void *params) {
  printf("运行主页面\n");
  // 解析配置文件
  if (parse_config_file("/etc/test_gui/gui.conf")) {
    printf("parse_config_file err\n");
    return;
  };

  // 生成按钮和页面
  if (generate_buttons()) {
    printf("generate_buttons err\n");
    return;
  };

  // 获取InputEvent事件并拿到对应的button并执行相应的onPress函数
  InputEvent input_event;
  ButtonPtr button_ptr = NULL;
  int error;
  DispInfo disp_info;
  get_disp_info(&disp_info);

  while (1) {
    if ((error = get_input_event(&input_event))) {
      continue;
    }

    if (get_button_by_input_event(&input_event, &button_ptr)) {
      printf("get_button_by_input_event err\n");
      continue;
    }

    button_ptr->on_press(button_ptr, &disp_info, &input_event);
  }
}

static Page main_page = {
    .name = "main",
    .run = run,
};

void main_page_register(void) { page_register(&main_page); }
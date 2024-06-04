#include "../include/config.h"
#include <stdio.h>
#include <string.h>

static ItemConfig item_config_arr[MAX_ITEM_COUNT];
static int item_count = 0;

int parse_config_file(char *filepathname) {
  FILE *file_ptr;
  char buf[100];
  char *p;
  // 打开文本文件
  if (!(file_ptr = fopen(filepathname, "r"))) {
    printf("fopen err\n");
    return -1;
  }

  // 读取每一行，并存入到item_config_arr中
  while (fgets(buf, 100, file_ptr)) {
    // 移除掉前面的空格或TAB
    p = buf;
    while (*p == ' ' || *p == '\t')
      p++;

    // 忽略注释行
    if (*p == '#')
      continue;

    item_config_arr[item_count].command[0] = '\0';
    item_config_arr[item_count].index = item_count;
    sscanf(p, "%s %d %s", item_config_arr[item_count].name,
           &item_config_arr[item_count].is_enabled_touch,
           item_config_arr[item_count].command);
    item_count++;
  }

  // 打印
  for (int i = 0; i < item_count; i++) {
    printf("name: %s, is_enabled_touch: %d, command: %s\n",
           item_config_arr[i].name, item_config_arr[i].is_enabled_touch,
           item_config_arr[i].command);
  }

  printf("配置文件解析成功\n");

  return 0;
}

int get_item_count(void) { return item_count; }

ItemConfigPtr get_item_by_index(int index) {
  if (index >= item_count) {
    return NULL;
  }
  return &item_config_arr[index];
}

ItemConfigPtr get_item_by_name(const char *name) {
  for (int i = 0; i < item_count; i++) {
    if (strcmp(item_config_arr[i].name, name) == 0) {
      return &item_config_arr[i];
    }
  }
  return NULL;
}

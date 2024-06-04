#ifndef CONFIG_H
#define CONFIG_H

#define MAX_ITEM_COUNT 100

typedef struct ItemConfig {
  int index;
  char name[100];
  int is_enabled_touch;
  char command[100];
} ItemConfig, *ItemConfigPtr;

int parse_config_file(char *filepathname);
int get_item_count(void);
ItemConfigPtr get_item_by_index(int index);
ItemConfigPtr get_item_by_name(const char *name);

#endif
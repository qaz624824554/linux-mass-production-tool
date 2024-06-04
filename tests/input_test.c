#include "../include/input_manager.h"
#include <stdio.h>

int main(void) {
  int ret;
  InputEvent input_event;

  input_init();

  input_device_init();

  while (1) {
    if ((ret = get_input_event(&input_event)) == 0) {
      if (input_event.type == INPUT_TYPE_TOUCH) {
        printf("type: %d\n", input_event.type);
        printf("x: %d\n", input_event.x);
        printf("y: %d\n", input_event.y);
        printf("pressure: %d\n", input_event.pressure);
        printf("------------------\n");
      } else if (input_event.type == INPUT_TYPE_NET) {
        printf("time: %ld\n", input_event.t_time.tv_sec);
        printf("str: %s\n", input_event.str);
        printf("--------------\n");
      }
    }
  }
}
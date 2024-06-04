#include "../include/input_manager.h"
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <tslib.h>

static struct tsdev *ts;

int touch_screen_get_input_event(InputEventPtr input_event_ptr) {
  struct ts_sample samp;
  int ret;
  ret = ts_read(ts, &samp, 1);

  if (ret != 1 || samp.pressure == 0) {
    return -1;
  }

  input_event_ptr->x = samp.x;
  input_event_ptr->y = samp.y;
  input_event_ptr->type = INPUT_TYPE_TOUCH;
  input_event_ptr->pressure = samp.pressure;
  input_event_ptr->t_time = samp.tv;

  return 0;
};
int touch_screen_device_init(void) {
  // 打开设备
  if (!(ts = ts_setup(NULL, 0))) {
    printf("ts_setup err\r");
    return -1;
  }
  return 0;
};
int touch_screen_device_exit(void) {
  ts_close(ts);
  return 0;
};

static struct InputDevice TouchScreen = {
    .name = "touchscreen",
    .get_input_event = touch_screen_get_input_event,
    .device_init = touch_screen_device_init,
    .device_exit = touch_screen_device_exit,
};

void register_touch_screen(void) { register_device(&TouchScreen); }

// #if 1

// int main(void) {
//   TouchScreen.device_init();
//   InputEvent input_event;
//   int ret;

//   while (1) {
//     ret = TouchScreen.get_input_event(&input_event);
//     if (ret < 0) {
//       printf("read error");
//       TouchScreen.device_exit();
//       exit(EXIT_FAILURE);
//     }
//     printf("type: %d\n", input_event.type);
//     printf("x: %d\n", input_event.x);
//     printf("y: %d\n", input_event.y);
//     printf("pressure: %d\n", input_event.pressure);
//     printf("------------------\n");
//   }

//   TouchScreen.device_exit();
// }

// #endif
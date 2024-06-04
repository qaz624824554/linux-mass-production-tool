#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <bits/types/struct_timeval.h>

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET 2

typedef struct InputEvent {
  struct timeval t_time;
  int type;
  int x;
  int y;
  int pressure;
  char str[1024];
} InputEvent, *InputEventPtr;

typedef struct InputDevice {
  char *name;
  int (*get_input_event)(InputEventPtr);
  int (*device_init)(void);
  int (*device_exit)(void);
  struct InputDevice *next;
} InputDevice, *InputDevicePtr;

void register_device(InputDevicePtr idp);
void input_init(void);
void input_device_init(void);
void input_device_exit(void);
int get_input_event(InputEventPtr input_event_ptr);

#endif
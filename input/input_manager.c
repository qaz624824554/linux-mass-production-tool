#include "../include/input_manager.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// 环形缓冲区
#define CYCLE_BUFFER_SIZE 20
static int read_index;
static int write_index;
static InputEvent InputEventBuf[CYCLE_BUFFER_SIZE];

static int isFull(void) {
  return read_index == (write_index + 1) % CYCLE_BUFFER_SIZE;
}
static int isEmpty(void) { return read_index == write_index; }

static void putData(InputEventPtr input_event_ptr) {
  if (!isFull()) {
    InputEventBuf[write_index] = *input_event_ptr;
    write_index = (write_index + 1) % CYCLE_BUFFER_SIZE;
  }
}

static int getData(InputEventPtr input_event_ptr) {
  if (!isEmpty()) {
    *input_event_ptr = InputEventBuf[read_index];
    read_index = (read_index + 1) % CYCLE_BUFFER_SIZE;
    return 0;
  }
  return -1;
}

static pthread_mutex_t mutex;
static pthread_cond_t cond;

void *thread_handler(void *data) {
  InputDevicePtr input_device_ptr = (InputDevicePtr)data;
  InputEvent input_event;
  while (1) {
    if (input_device_ptr->get_input_event(&input_event)) {
      continue;
    };
    pthread_mutex_lock(&mutex);
    putData(&input_event);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

InputDevicePtr input_device_ptr = NULL;

void register_device(InputDevicePtr idp) {
  idp->next = input_device_ptr;
  input_device_ptr = idp;
}

void input_init(void) {
  // register devices
  extern void register_touch_screen(void);
  extern void register_net(void);

  register_touch_screen();
  register_net();
}

void input_device_init(void) {
  // init devices
  InputDevicePtr tmp_p = input_device_ptr;
  int ret;
  pthread_t pid;
  while (tmp_p) {
    ret = tmp_p->device_init();
    if (ret < 0) {
      printf("%s 设备注册失败\n", tmp_p->name);
      return;
    }
    pthread_create(&pid, NULL, thread_handler, tmp_p);
    tmp_p = tmp_p->next;
  }
}

void input_device_exit(void) {
  // exit devices
  InputDevicePtr tmp_p = input_device_ptr;
  while (tmp_p) {
    tmp_p->device_exit();
    tmp_p = tmp_p->next;
  }
}

int get_input_event(InputEventPtr input_event_ptr) {
  pthread_mutex_lock(&mutex);
  int ret;
  // 有数据，释放锁，无数据，让出锁，休眠，等待唤醒
  if ((ret = getData(input_event_ptr)) == 0) {
    pthread_mutex_unlock(&mutex);
    return ret;
  } else {
    pthread_cond_wait(&cond, &mutex);
    if ((ret = getData(input_event_ptr)) == 0) {
    }
    pthread_mutex_unlock(&mutex);
  }
  return ret;
}
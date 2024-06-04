#include "../include/input_manager.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

#define SERVER_PORT 8888

static int sockfd;

static int net_get_input_event(InputEventPtr input_event_ptr) {
  unsigned int client_addr_len = sizeof(struct sockaddr);
  struct sockaddr_in client_addr;
  char buf[1024];
  // 接收数据并打印
  int recv_len = recvfrom(sockfd, buf, sizeof(buf), 0,
                          (struct sockaddr *)&client_addr, &client_addr_len);
  if (recv_len > 0) {
    buf[recv_len] = '\0';
    input_event_ptr->type = INPUT_TYPE_NET;
    strcpy(input_event_ptr->str, buf);
    gettimeofday(&input_event_ptr->t_time, NULL);
    return 0;
  }
  printf("接收数据失败\n");
  return -1;
};

static int net_device_init(void) {
  struct sockaddr_in addr;
  // 创建socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  // 绑定socket
  memset(&addr, 0, sizeof(addr));
  addr.sin_port = htons(SERVER_PORT);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  if ((bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
    perror("bind");
    return -1;
  }

  return 0;
};

static int net_device_exit(void) {
  close(sockfd);
  return 0;
};

static InputDevice Net = {
    .name = "net",
    .get_input_event = net_get_input_event,
    .device_init = net_device_init,
    .device_exit = net_device_exit,
};

void register_net(void) {
  register_device(&Net);
}

// #if 1
// int main(void) {
//   Net.device_init();

//   InputEvent input_event;

//   while (1) {
//     Net.get_input_event(&input_event);
//     printf("time: %ld\n", input_event.t_time.tv_sec);
//     printf("str: %s\n", input_event.str);
//     printf("--------------\n");
//   }
// }

// #endif
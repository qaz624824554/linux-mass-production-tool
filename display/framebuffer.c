#include "../include/disp_manager.h"
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

static int fd_fb;
static struct fb_var_screeninfo fb_info;
static unsigned char *fb_base;
static int screen_size;
static int pixel_width;
static int line_width;

int framebuffer_init(void) {
  if ((fd_fb = open("/dev/fb0", O_RDWR)) == -1) {
    perror("open fail");
    return 1;
  }

  // 使用ioctl获取framebuffer驱动的输出数据
  if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &fb_info) == -1) {
    perror("ioctl fail");
    return 1;
  }

  printf("屏幕分辨率： %d × %d\n", fb_info.xres, fb_info.yres);
  printf("每个像素占用: %d 位\n", fb_info.bits_per_pixel);

  // 计算fb需要的字节单位空间，将/dev/fb0字符设备文件映射到内存区域
  pixel_width = fb_info.bits_per_pixel / 8;
  screen_size = fb_info.xres * fb_info.yres * pixel_width;
  line_width = fb_info.xres * pixel_width;
  printf("每像素的大小为: %d 字节\n", pixel_width);
  printf("整个屏幕缓冲区大小为: %d 字节\n", screen_size);
  printf("一行有: %d 个字节\n", line_width);
  fb_base = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE,
                                  MAP_SHARED, fd_fb, 0);

  if (fb_base == MAP_FAILED) {
    perror("mmap");
    return 1;
  }
  
  // 修改该内存区域的值，使其显示相应的颜色
  memset(fb_base, 0x00, screen_size);

  return 0;
}
int framebuffer_exit(void) {
  // 关闭文件描述符，关闭内存映射，释放资源
  munmap(fb_base, screen_size);
  close(fd_fb);
  return 0;
}

void combine_disp_info(DispInfoPtr p) {
  p->x_res = fb_info.xres;
  p->y_res = fb_info.yres;
  p->bpp = fb_info.bits_per_pixel;
  p->fb_base = fb_base;
  p->line_width = line_width;
  p->pixel_width = pixel_width;
  p->screen_size = screen_size;
};

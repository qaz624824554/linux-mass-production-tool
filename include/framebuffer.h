#include "disp_manager.h"

#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

int framebuffer_init(void);
int framebuffer_exit(void);
void combine_disp_info(DispInfoPtr p);

#endif
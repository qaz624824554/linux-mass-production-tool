#include "../include/page_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PagePtr page_ptr = NULL;

void pages_register(void) {
  extern void main_page_register(void);
  main_page_register();
}

PagePtr page(char *name) {
  PagePtr ptr = page_ptr;
  while (ptr) {
    if ((strcmp(name, ptr->name)) == 0) {
      printf("页面匹配成功\n");
      return ptr;
    }
    ptr = ptr->next;
  }
  return NULL;
}


void page_register(PagePtr ptr) {
  ptr->next = page_ptr;
  page_ptr = ptr;
}
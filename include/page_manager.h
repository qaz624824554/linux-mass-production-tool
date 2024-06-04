#ifndef PAGE_H
#define PAGE_H

typedef struct Page {
  char *name;
  void (*run)(void *params);
  struct Page *next;
} Page, *PagePtr;

void pages_register(void);
PagePtr page(char *name);
void page_register(PagePtr ptr);

#endif
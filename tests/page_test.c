#include <stdio.h>

#include "../include/page_manager.h"

int main(void)
{
  pages_register();

  page("main")->run(NULL);

  return 0;
}

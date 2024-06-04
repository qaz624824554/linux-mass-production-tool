#include "../include/disp_manager.h"
#include "../include/ui.h"

static int on_default_draw(struct Button *button_ptr,
                           struct DispInfo *disp_info_ptr) {
  draw_region(button_ptr->region, BUTTON_BG_COLOR);

  set_font_size(button_ptr->font_size);

  draw_text_in_region_central(button_ptr->name, button_ptr->region,
                              BUTTON_TEXT_COLOR);

  flush_region(&button_ptr->region, disp_info_ptr->fb_base);

  return 0;
};

static int on_default_press(ButtonPtr button_ptr, DispInfoPtr disp_info_ptr,
                            InputEventPtr input_event_ptr) {
  int bg_color = BUTTON_BG_COLOR;
  button_ptr->status = !button_ptr->status;

  if (button_ptr->status) {
    bg_color = BUTTON_BG_PRESSED_COLOR;
  }

  draw_region(button_ptr->region, bg_color);

  draw_text_in_region_central(button_ptr->name, button_ptr->region,
                              BUTTON_TEXT_COLOR);

  flush_region(&button_ptr->region, disp_info_ptr->fb_base);

  return 0;
};

void init_button(ButtonPtr button_ptr, char *name, RegionPtr region_ptr,
                 OnDraw on_draw, OnPress on_press) {

  button_ptr->name = name;
  button_ptr->status = 0;
  if (region_ptr) {
    button_ptr->region = *region_ptr;
  }
  button_ptr->on_draw = on_draw ? on_draw : on_default_draw;
  button_ptr->on_press = on_press ? on_press : on_default_press;
}
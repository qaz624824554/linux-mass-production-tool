#include <fcntl.h>
#include <ft2build.h>
#include <stdio.h>
#include <unistd.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "../include/font_manager.h"

static FT_Library library;
static FT_Face face;

static int freetype_font_init(char *filepathname) {

  int error = FT_Init_FreeType(&library);

  error = FT_New_Face(library, filepathname, 0, &face);

  return error;
}

static void freetype_set_font_size(int size) {
  FT_Set_Pixel_Sizes(face, size, 0);
}

static int freetype_get_font_bit_map(unsigned int char_code,
                                     FontBitMapPtr font_bit_map_ptr) {
  int error;
  FT_Vector pen;
  FT_GlyphSlot slot = face->glyph;

  // 将当前的绘制位置转换为 FreeType 使用的 1/64 像素单位
  pen.x = font_bit_map_ptr->cur_origin_x * 64;
  pen.y = font_bit_map_ptr->cur_origin_y * 64;

  FT_Set_Transform(face, 0, &pen);

  if ((error = FT_Load_Char(face, char_code, FT_LOAD_RENDER))) {
    printf("FT_Load_Char error\n");
    return -1;
  }

  font_bit_map_ptr->font_buffer = slot->bitmap.buffer;
  font_bit_map_ptr->region.left_top_x = slot->bitmap_left;
  font_bit_map_ptr->region.left_top_y =
      font_bit_map_ptr->cur_origin_y * 2 - slot->bitmap_top;
  font_bit_map_ptr->region.width = slot->bitmap.width;
  font_bit_map_ptr->region.height = slot->bitmap.rows;
  font_bit_map_ptr->next_origin_x =
      font_bit_map_ptr->cur_origin_x + slot->advance.x / 64;
  font_bit_map_ptr->next_origin_y = font_bit_map_ptr->cur_origin_y;

  return 0;
}

static int
freetype_get_string_region_car(char *str,
                               RegionCartesianPtr region_cartesian_p) {
  int i;
  int error;
  FT_BBox bbox;
  FT_BBox glyph_bbox;
  FT_Vector pen;
  FT_Glyph glyph;
  FT_GlyphSlot slot = face->glyph;

  /* 初始化 */
  bbox.xMin = bbox.yMin = 32000;
  bbox.xMax = bbox.yMax = -32000;

  /* 指定原点为(0, 0) */
  pen.x = 0;
  pen.y = 0;

  /* 计算每个字符的bounding box */
  /* 先translate, 再load char, 就可以得到它的外框了 */
  for (i = 0; i < strlen(str); i++) {
    /* 转换：transformation */
    FT_Set_Transform(face, 0, &pen);

    /* 加载位图: load glyph image into the slot (erase previous one) */
    error = FT_Load_Char(face, str[i], FT_LOAD_RENDER);
    if (error) {
      printf("FT_Load_Char error\n");
      return -1;
    }

    /* 取出glyph */
    error = FT_Get_Glyph(face->glyph, &glyph);
    if (error) {
      printf("FT_Get_Glyph error!\n");
      return -1;
    }

    /* 从glyph得到外框: bbox */
    FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

    /* 更新外框 */
    if (glyph_bbox.xMin < bbox.xMin)
      bbox.xMin = glyph_bbox.xMin;

    if (glyph_bbox.yMin < bbox.yMin)
      bbox.yMin = glyph_bbox.yMin;

    if (glyph_bbox.xMax > bbox.xMax)
      bbox.xMax = glyph_bbox.xMax;

    if (glyph_bbox.yMax > bbox.yMax)
      bbox.yMax = glyph_bbox.yMax;

    /* 计算下一个字符的原点: increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  /* return string bbox */
  //*abbox = bbox;
  region_cartesian_p->left_top_x = bbox.xMin;
  region_cartesian_p->left_top_y = bbox.yMax;
  region_cartesian_p->width = bbox.xMax - bbox.xMin + 1;
  region_cartesian_p->height = bbox.yMax - bbox.yMin + 1;

  return 0;
}

FontOpr freetype = {.name = "freetype",
                    .font_init = freetype_font_init,
                    .set_font_size = freetype_set_font_size,
                    .get_font_bit_map = freetype_get_font_bit_map,
                    .get_string_region_car = freetype_get_string_region_car};

void register_freetype_font() { register_fonts(&freetype); }
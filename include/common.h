#ifndef COMMON_H
#define COMMON_H

typedef struct Region {
  int left_top_x;
  int left_top_y;
  int width;
  int height;
} Region, *RegionPtr;

typedef struct RegionCartesian {
  int left_top_x;
  int left_top_y;
  int width;
  int height;
} RegionCartesian, *RegionCartesianPtr;

#endif
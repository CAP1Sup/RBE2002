#pragma once

#include <Romi32U4.h>

#define MEDIAN_FILTER_SIZE 5

class MedianFilter {
 private:
  int array[MEDIAN_FILTER_SIZE] = {0};

 public:
  void Sort(int, int);
  void Init(void);
  int Filter(int);
};

#include "MedianFilter.cpp"
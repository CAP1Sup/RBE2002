#pragma once

#include <Romi32U4.h>

template <typename T>
class MedianFilter
{
private:
  T *values;
  uint8_t totalValues;
  uint8_t readings = 0;
  uint8_t currentIndex = 0;

public:
  /**
   * @brief Constructs a new Median Filter object with a given type and size
   * Size must be <= 255!
   *
   * @param size
   */
  MedianFilter<T>(const uint8_t size);

  /**
   * @brief Adds a value to the filter
   *
   * @param value The value to add to the filter
   */
  void addValue(T value);

  /**
   * @brief Get the Average object
   *
   * @return T
   */
  T getAverage();

  /**
   * @brief Returns the median value of the filter
   *
   * @return T The median value
   */
  T getMedian();
};

#include "MedianFilter.cpp"
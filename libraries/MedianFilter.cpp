#include "MedianFilter.h"

#include <KickSort.h>

template <typename T>
MedianFilter<T>::MedianFilter(const uint8_t size) {
  values = new T[size];
  totalValues = size;
}

template <typename T>
void MedianFilter<T>::addValue(T value) {
  values[currentIndex] = value;
  currentIndex++;
  if (currentIndex >= totalValues) {
    currentIndex = 0;
  }
  if (readings < totalValues) {
    readings++;
  }
}

template <typename T>
T MedianFilter<T>::getAverage() {
  float sum = 0;
  for (uint8_t i = 0; i < totalValues; i++) {
    sum += values[i];
  }
  return sum / totalValues;
}

template <typename T>
T MedianFilter<T>::getMedian() {
  T tempValues[readings];
  for (uint8_t i = 0; i < readings; i++) {
    tempValues[i] = values[i];
  }
  KickSort<int16_t>::quickSort(tempValues, readings);
  if (readings % 2 == 0) {
    return (tempValues[readings / 2] + tempValues[readings / 2 - 1]) / 2;
  } else {
    return tempValues[readings / 2];
  }
}

template <typename T>
void MedianFilter<T>::purge() {
  readings = 0;
  currentIndex = 0;
}

template <typename T>
bool MedianFilter<T>::isFull() {
  return readings >= totalValues;
}
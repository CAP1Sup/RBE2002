#include "MedianFilter.h"

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
T MedianFilter<T>::getMedian() {
  T tempValues[readings];
  for (uint8_t i = 0; i < readings; i++) {
    tempValues[i] = values[i];
  }
  for (uint8_t i = 0; i < readings; i++) {
    for (uint8_t j = i + 1; j < readings; j++) {
      if (tempValues[j] < tempValues[i]) {
        T temp = tempValues[i];
        tempValues[i] = tempValues[j];
        tempValues[j] = temp;
      }
    }
  }
  if (readings % 2 == 0) {
    return (tempValues[readings / 2] + tempValues[readings / 2 - 1]) / 2;
  } else {
    return tempValues[readings / 2];
  }
}
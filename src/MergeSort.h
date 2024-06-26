//
// Created by 13199 on 2024/5/30.
//

#ifndef TICKETSYSTEM_MERGESORT_H
#define TICKETSYSTEM_MERGESORT_H

#include "vector/vector.hpp"

template<typename T, typename Compare>
void merge(sjtu::vector<T>& arr, size_t left, size_t mid, size_t right, Compare comp) {
  sjtu::vector<T> leftArr;
  sjtu::vector<T> rightArr;

  for (size_t i = left; i < mid + 1; ++i) {
    leftArr.push_back(arr[i]);
  }
  for (size_t i = mid + 1; i < right + 1; ++i) {
    rightArr.push_back(arr[i]);
  }

  size_t i = 0, j = 0, k = left;
  while (i < leftArr.size() && j < rightArr.size()) {
    if (comp(leftArr[i], rightArr[j])) {
      arr[k++] = leftArr[i++];
    } else {
      arr[k++] = rightArr[j++];
    }
  }

  while (i < leftArr.size()) {
    arr[k++] = leftArr[i++];
  }
  while (j < rightArr.size()) {
    arr[k++] = rightArr[j++];
  }
}

// 归并排序函数
template<typename T, typename Compare>
void mergeSort(sjtu::vector<T>& arr, size_t left, size_t right, Compare comp) {
  if (left < right) {
    size_t mid = (left + right) / 2;
    mergeSort(arr, left, mid, comp);
    mergeSort(arr, mid + 1, right, comp);
    merge(arr, left, mid, right, comp);
  }
}

// 包装函数
template<typename T, typename Compare>
void mergeSort(sjtu::vector<T>& arr, Compare comp) {
  mergeSort(arr, 0, arr.size() - 1, comp);
}

#endif //TICKETSYSTEM_MERGESORT_H

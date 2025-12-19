#include "contract.h"
#include <stdlib.h>

//вычисление pi по ряду лейбница
API float Pi(int k) {
    if (k <= 0) return 0.0f;
    double sum = 0.0;
    for (int i = 0; i < k; ++i) {
        double term = 1.0 / (2 * (double)i + 1.0);
        if (i % 2 == 0) sum += term;
        else sum -= term;
    }
    return (float)(4.0 * sum);
}

//пузырьковая сортировка
API int* Sort(int* array, int size) {
    if (!array || size <= 1) return array;
    for (int i = 0; i < size - 1; ++i) {
        int swapped = 0;
        for (int j = 0; j < size - 1 - i; ++j) {
            if (array[j] > array[j+1]) {
                int t = array[j];
                array[j] = array[j+1];
                array[j+1] = t;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    return array;
}

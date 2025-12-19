#include "contract.h"
#include <stdlib.h>
#include <math.h>

//вычисление pi по Валлису
API float Pi(int k) {
    if (k <= 0) return 0.0f;
    
    double product = 1.0;
    for (int i = 1; i <= k; ++i) {
        double numerator = 4.0 * i * i;
        double denominator = 4.0 * i * i - 1.0;
        product *= numerator / denominator;
    }
    
    return (float)(2.0 * product);
}

//вспомогательная функция для хоарра
static int partition(int* array, int low, int high) {
    int pivot = array[(low + high) / 2]; 
    int i = low - 1;
    int j = high + 1;
    
    while (1) {
        do {
            i++;
        } while (array[i] < pivot);
        
        do {
            j--;
        } while (array[j] > pivot);
        
        if (i >= j) {
            return j;
        }
        
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

static void quick_sort(int* array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quick_sort(array, low, pi);
        quick_sort(array, pi + 1, high);
    }
}

API int* Sort(int* array, int size) {
    if (!array || size <= 1) return array;
    
    quick_sort(array, 0, size - 1);
    return array;
}
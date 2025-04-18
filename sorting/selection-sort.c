#include <stdio.h>

void swap(int arr[], int a, int b) {
    // arr[a] ^= arr[b] ^= arr[a] ^= arr[b];   

    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int locOfSmallest(int arr[], int start, int end) {
    int i = start;
    int j = i;

    while(i <= end) {
        if(arr[i] < arr[j])
            j = i;

        i++;
    }

    return j;
}

void selection_sort(int* arr, int size) {
    if(!arr || size <= 1) 
        return;

    int i = 0;
    while(i < size - 1) {
        int j = locOfSmallest(arr, i, size - 1);
        swap(arr, i, j);

        i++;
    }
}

int main() {
    int arr[] = {2, 3, 1, 5};
    int length = sizeof(arr) / sizeof(int);

    for(int i = 0; i < length; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    printf("\n");

    selection_sort(arr, length);

    for(int i = 0; i < length; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    printf("\n");

    return 0;
}
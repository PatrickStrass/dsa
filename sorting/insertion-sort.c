// void insertion_sort(int* array, int size) {
//     if(!array || size <= 0)
//         return;
    
//     for(int i = 1; i < size; i++) {
//         int key = array[i];

//         // insert array[i] into the sorted subarray array[0:i-1]
//         int j = i - 1;
//         while(j >= 0 && array[j] > key) {
//             array[j + 1] = array[j];
//             j--;
//         }

//         array[j + 1] = key;
//     }
// }

void insertion_sort(int* array, int size) {
    if(!array || size <= 1)
        return;

    for(int i = 0; i < size; i++) {
        for(int j = i + 1; j < size; j++) {
            if(array[j] < array[i]) 
                array[j] ^= array[i] ^= array[j] ^= array[i];
            
        }
    }
}

int main() {
    int unsorted[] = {2, 3, 1, 5};
    int length = sizeof(unsorted) / sizeof(unsorted[0]);

    for(int i = 0; i < length; i++) {
        printf("array[%d] = %d\n", i, unsorted[i]);
    }
    printf("\n");
    
    insertion_sort(unsorted, length);

    for(int i = 0; i < length; i++) {
        printf("array[%d] = %d\n", i, unsorted[i]);
    }
}
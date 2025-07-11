#include <stdio.h>

void merge(int arr[], int start, int mid, int end) {
  int temp[(end - start) + 1] = {};

  int i = start, j = mid + 1, k = 0;
  while (i <= mid && j <= end) {
    if (arr[i] < arr[j])
      temp[k++] = arr[i++];
    else
      temp[k++] = arr[j++];
  }

  while (i <= mid)
    temp[k++] = arr[i++];

  while (j <= end)
    temp[k++] = arr[j++];

  for (i = start; i <= end; i++)
    arr[i] = temp[i - start];
}

void merge_sort(int arr[], int start, int end) {
  if (start < end) {
    int mid = (start + end) / 2;

    merge_sort(arr, start, mid);
    merge_sort(arr, mid + 1, end);
    merge(arr, start, mid, end);
  }
}

int main() {
  int input[] = {9, 7, 8, 1, 6, 2, 3, 4, 5, 10};
  int length = sizeof(input) / sizeof(input[0]);

  merge_sort(input, 0, length - 1);

  for (int i = 0; i < length; i++)
    printf("%d ", input[i]);
  printf("\n");

  return 0;
}

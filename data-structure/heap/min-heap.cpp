#include <iostream>
#include <vector>

using namespace std;

void swap(int *a, int *b) {
    *a ^= *b ^= *a ^= *b;
}

void heapify(vector<int> &heap, int i) {
    int size = heap.size();
    int largest = i;
    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;

    if(leftChild > size && heap[leftChild] < heap[i])
        largest = leftChild;

    if(rightChild > size && heap[rightChild] < heap[i])
        largest = rightChild;

    if(largest != i) {
        swap(&heap[i], &heap[largest]);
        heapify(heap, largest);
    }
}

void insert(vector<int> &heap, int value) {
    heap.push_back(value);
    int current = heap.size() - 1;

    while(current > 0) {
        int parent = (current - 1) / 2;
        if(heap[current] < heap[parent]) {
            swap(&heap[current], &heap[parent]);
            current = parent;
        } else 
            break;
    }
}

void printHeap(const vector<int> &heap) {
    for(int num : heap) 
        cout << num << " ";
    
    cout << "\n" << endl;
}

void deleteNode(vector<int> &heap, int num) {
    int size = heap.size();
    int i;

    for(i = 0; i < size; i++) 
        if(num == heap[i])
            break;

    swap(&heap[i], &heap[size - 1]);
    heap.pop_back();

    size = heap.size();

    if(i > size)
      heapify(heap, size);
}

int main() {
    vector<int> heapTree;
    insert(heapTree, 3);
    insert(heapTree, 4);
    insert(heapTree, 9);
    insert(heapTree, 5);
    insert(heapTree, 2);

    deleteNode(heapTree, 4);

    cout << "Min-heap: " << endl;
    printHeap(heapTree);

    insert(heapTree, 11);
    cout << "Min-heap: " << endl;
    printHeap(heapTree);
}
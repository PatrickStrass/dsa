#include <iostream>

using namespace std;

class Node {
    public:
        int data;
        Node *next;
};

class LinkedList {
    private:
        Node *head;

    public:
        LinkedList() {head = NULL;}
        LinkedList(int arr[], int size);
        ~LinkedList();

        void display();
        void insertAt(int position, int value);
        int deleteAt(int position);
        int length();
        int max();
        int min();
        int sum();
};

LinkedList::LinkedList(int arr[], int size) {
    Node *tail;
    Node *temp;

    head = new Node();
    head->data = arr[0];
    head->next = NULL;
    tail = head;

    for(int i = 1; i < size; i++) {
        temp = new Node();
        temp->data = arr[i];
        temp->next = NULL;
        tail->next = temp;
        tail = temp;
    }
}

LinkedList::~LinkedList() { // testar
    Node *node = head;

    while (head != NULL) {
        head = head->next;
        delete node;
        node = head;
    }
}

void LinkedList::display() {
    Node *node = head;

    while(node != NULL) {
        cout << node->data << " ";
        node = node->next;
    }

    cout << endl;
}

int LinkedList::length() {
    int length = 0;
    Node *node = head;

    while(node != NULL) {
        length++;
        node = node->next;
    }

    return length;
}

void LinkedList::insertAt(int position, int value) {
    Node *temp = NULL;
    Node *node = head;

    if(position < 0 || position > length()) 
        return;

    temp = new Node();
    temp->data = value;
    temp->next = NULL;

    if(position == 0) {
        head = temp;
    } else {
        for(int i = 0; i < position -1; i++)
            node = node->next;
        
        temp->next = node->next;
        node->next = temp;
    }
}

int LinkedList::deleteAt(int position) {
    Node *node = NULL;
    Node *trailing = NULL;
    int value = -1;

    if(position < 0 || position > length()) 
        return value;

    if(position == 0) {
        node = head;
        head = head->next;
        value = node->data;
        delete node;
    } else {
        node = head;

        for(int i = 0; i < position - 1; i++) {
            trailing = node;
            node = node->next;
        }

        trailing->next = node->next;
        value = node->data;
        delete node;
    }

    return value;
}

int LinkedList::max() {
    int max = INT16_MIN;
    Node *node = head;

    while(node != NULL) {
        if(node->data > max)
            max = node->data;

        node = node->next;
    }

    return max;
}

int LinkedList::min() {
    int min = INT16_MAX;
    Node *node = head;

    while(node != NULL) {
        if(node->data < min)
            min = node->data;

        node = node->next;
    }

    return min;
}

int LinkedList::sum() {
    int sum = 0;
    Node *node = head;

    while(node != NULL) {
        sum += node->data;
        node = node->next;
    }

    return sum;
}

int main() {
    int A[] = {10, 20, 30, 40, 50};
    LinkedList lista(A, 5);

    lista.display();
    cout << "A lista possui " << lista.length() << " elementos" << endl;

    lista.insertAt(4, 99);
    cout << "Após a inserção, a lista está assim: ";
    lista.display();

    cout << "O elemento de valor " << lista.deleteAt(3) << " foi deletado" << endl;
    lista.display();

    cout << "Maior elemento da lista: " << lista.max() << endl;

    cout << "Menor elemento da lista: " << lista.min() << endl;

    cout << "Soma dos elementos da lista: " << lista.sum() << endl;
    
    return 0;
}
#include <iostream>

using namespace std;

class Node {
    public:
        int data;
        Node *previous;
        Node *next;
};

class DoublyLinkedList {
    private:
        Node *head;

    public:
        DoublyLinkedList() {head = NULL;}
        DoublyLinkedList(int arr[], int size);
        ~DoublyLinkedList();

        void display();
        void insertAt(int position, int value);
        int deleteAt(int position);
        int length();
        int max();
        int min();
        int sum();
};

DoublyLinkedList::DoublyLinkedList(int arr[], int size) {
    Node *node;
    Node *temp;

    head = new Node();
    head->data = arr[0];
    head->next = NULL;
    head->previous = NULL;
    node = head;

    for(int i = 1; i < size; i++) {
        temp = new Node();
        temp->data = arr[i];
        node->next = temp;
        temp->previous = node;
        node = node->next;
    }
}

DoublyLinkedList::~DoublyLinkedList() {
    Node *node = head;

    while(node != NULL) {
        head = head->next;
        delete node;
        node = head;
    }
}

void DoublyLinkedList::display() {
    Node *node = head;

    while(node != NULL) {
        cout << node->data << " ";
        node = node->next;
    }

    cout << endl;
}

void DoublyLinkedList::insertAt(int position, int value) {
    Node *temp = NULL;
    Node *node = head;

    if(position < 0 || position > length()) 
        return;

    temp = new Node();
    temp->data = value;
    temp->next = NULL;
    temp->previous = NULL;

    if(position == 0) {
        temp->next = head;
        head->previous = temp;
        head = temp;
    } else {
        for(int i = 0; i < position - 1; i++)
            node = node->next;
        
        temp->next = node->next;
        temp->previous = node;
        node->next->previous = temp;
        node->next = temp;
    }
}

int DoublyLinkedList::deleteAt(int position) {
    Node *node = NULL;
    Node *trailing = NULL;
    int value = -1;

    if(position < 0 || position > length()) 
        return value;

    if(position == 0) {
        node = head;
        head = head->next;
        head->previous = NULL;
        value = node->data;
        delete node;
    } else {
        node = head;

        for(int i = 0; i < position; i++) {
            trailing = node;
            node = node->next;
        }

        if(node->next != NULL) {
            trailing->next = node->next;
            node->next->previous = trailing;
            node->next = trailing;
        } else {
            trailing->next = NULL;
            node->previous = NULL;
        }
        
        value = node->data;
        delete node;
    }

    return value;
}

int DoublyLinkedList::length() {
    int length = 0;
    Node *node = head;

    while(node != NULL) {
        length++;
        node = node->next;
    }

    return length;
}

int DoublyLinkedList::max() {
    int max = INT16_MIN;
    Node *node = head;

    while(node != NULL) {
        if(node->data > max)
            max = node->data;

        node = node->next;
    }

    return max;
}

int DoublyLinkedList::min() {
    int min = INT16_MAX;
    Node *node = head;

    while(node != NULL) {
        if(node->data < min)
            min = node->data;

        node = node->next;
    }

    return min;
}

int DoublyLinkedList::sum() {
    int sum = 0;
    Node *node = head;

    while(node != NULL) {
        sum += node->data;
        node = node->next;
    }

    return sum;
}

int main() {
    int arr[] = {10, 20, 30, 40, 50};
    DoublyLinkedList lista(arr, 5);

    lista.display();
    cout << "A lista possui " << lista.length() << " elementos" << endl;

    lista.insertAt(4, 7);
    cout << "Após a inserção, a lista está assim: ";
    lista.display();

    cout << "O elemento de valor " << lista.deleteAt(4) << " foi deletado" << endl;
    lista.display();

    cout << "Maior elemento da lista: " << lista.max() << endl;

    cout << "Menor elemento da lista: " << lista.min() << endl;

    cout << "Soma dos elementos da lista: " << lista.sum() << endl;

    return 0;
}
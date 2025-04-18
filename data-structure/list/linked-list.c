#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *head = NULL;

void create(int arr[], int size)
{
    int i;
    Node *temp;
    Node *last;
    head = (Node *)malloc(sizeof(Node));

    head->data = arr[0];
    head->next = NULL;
    last = head;

    for (i = 1; i < size; i++)
    {
        temp = (Node *)malloc(sizeof(Node));
        temp->data = arr[i];
        temp->next = NULL;
        last->next = temp;
        last = temp;
    }
}

void display(Node *node)
{
    while (node != NULL)
    {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

void recursiveDisplay(Node *node)
{
    if (node != NULL)
    {
        printf("%d ", node->data);
        recursiveDisplay(node->next);
    }
    else
    {
        printf("\n");
    }
}

int count(Node *node)
{
    int count = 0;

    while (node != NULL)
    {
        count++;
        node = node->next;
    }

    return count;
}

int max(Node *node)
{
    int max = INT_MIN;

    while (node != NULL)
    {
        if (node->data > max)
            max = node->data;

        node = node->next;
    }

    return max;
}

Node *search(Node *node, int value)
{
    Node *found = NULL;

    while (node != NULL)
    {
        if (node->data == value)
            found = node;

        node = node->next;
    }

    return found;
}

void insertAt(Node *node, int position, int value)
{
    if (position < 0 || position > count(node) - 1)
    {
        return;
    }

    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = value;
    temp->next = NULL;

    if (position == 0)
    {
        temp->next = head;
        head = temp;
    }
    else
    {
        int i;

        for (i = 1; i <= position - 1; i++)
        {
            node = node->next;
        }

        temp->next = node->next;
        node->next = temp;
    }
}

void insertSorted(Node *node, int value) {
    Node *trailing = NULL;
    Node *temp = (struct Node *)malloc(sizeof(Node));
    temp->data = value;
    temp->next = NULL;

    if (head == NULL)
        head = temp;
    else {
        while (node != NULL && node->data < value) {
            trailing = node;
            node = node->next;
        }

        if (node == head) {
            temp->next = head;
            head = temp;
        } else {
            temp->next = trailing->next;
            trailing->next = temp;
        }
    }
}

int deleteAt(Node *node, int position) {
    Node *trailing;
    int value = -1;

    if (position < 0 || position > count(node) - 1)
        return value;
    if (position == 0) {
        trailing = head;
        value = head->data; 
        head = head->next;
        free(trailing);
        return value;
    } else {
        for(int i = 0; i < position; i++) {
        trailing = node;
        node = node->next;
        }

        trailing->next = node->next;
        value = node->data;
        free(node);
        return value;
        }
}

// void insert(int data) {
//     Node *temp = (Node*) malloc(sizeof(Node));
//     temp->data = data;
//     temp->next = NULL;

//     if(head != NULL)
//         temp->next = head;

//     head = temp;
// }

// void insert_at(int position, int data) {
//     Node *temp = (Node*) malloc(sizeof(Node));
//     temp->data = data;
//     temp->next = NULL;

//     if(position == 0) {
//         temp->next = head;
//         head = temp;

//         return;
//     }

//     Node *temp2 = head;

//     for(int i = 0; i < position - 1; i++)
//         temp2 = temp2->next;

//     temp->next = temp2->next;
//     temp2->next = temp;
// }

// void print() {
//     Node *temp = head;
//     printf("Linked list:");

//     while(temp != NULL) {
//         printf(" %d", temp->data);
//         temp = temp->next;
//     }

//     printf("\n");
// }

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};
    int length = sizeof(arr) / sizeof(int);

    create(arr, length);
    display(head);
    // recursiveDisplay(head);
    // printf("%d\n", count(head));
    // printf("%d\n", max(head));
    // printf("%p\n", search(head, 17));
    // insertAt(head, 5, 100);
    // insertSorted(head, 25);
    printf("%d\n", deleteAt(head, 5));
    display(head);
}
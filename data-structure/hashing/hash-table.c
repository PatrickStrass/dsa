#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int key;
    char *value;
    struct Node *next;
} Node;

typedef struct HashTable {
    Node **buckets;
    int capacity;
    int size;
} HashTable;

HashTable* newHashTable(int capacity) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));

    if(table == NULL) return NULL;

    table->capacity = capacity;
    table->size = 0;
    table->buckets = (Node **)malloc(capacity * sizeof(Node));

    if(table->buckets == NULL) {
        free(table);

        return NULL;
    }
     
    for(int i = 0; i < capacity; i++) {
        table->buckets[i] = NULL;
    }

    return table;
}

int getBucketIndex(HashTable *hashTable, int key) {
    return key % hashTable->capacity;
}

bool isEmpty(HashTable *HashTable) {
    return HashTable->size == 0;
}

void put(HashTable *hashTable, int key, char *value) {
    Node *node = (Node *)malloc(sizeof(Node));

    if(node == NULL) return;

    node->key = key;
    node->value = value;
    node->next = NULL;

    int index = getBucketIndex(hashTable, key);
    Node *temp = hashTable->buckets[index];

    if(!temp) {
        hashTable->buckets[index] = node;
        hashTable->size++;
    } else {
        while(temp && temp->next) {
            if(temp->key == key) {
                temp->value = value;
                return;
            }

            temp = (Node *)temp->next;
        }

        temp->next = node;
        hashTable->size++;
    }
}

void display(HashTable *table) {
    Node *node;

    for(int i = 0; i < table->capacity; i++) {
        node = table->buckets[i];

        if(!node) 
            continue;

        printf("%d ->", i);

        while(node != NULL) {
            printf(" [%d, %s]", node->key, node->value);
            node = (Node *)node->next;
        }
        printf("\n");
    }
}

Node* get(HashTable *hashTable, int key) {
    if(!hashTable)
        return NULL;

    int index = getBucketIndex(hashTable, key);
    Node *node = hashTable->buckets[index];

    while(node) {
        if(node->key == key)
            return node;

        node = node->next;
    }

    return NULL;
}

void delete(HashTable *hashTable, int key) {
    if(!hashTable)
        return;
    
    int index = getBucketIndex(hashTable, key);
    Node *node = hashTable->buckets[index];
    Node *previous = NULL;

    while(node) {
        if(node->key == key) {
            if(previous)
                previous->next = node->next;
            else 
                hashTable->buckets[index] = node->next;
            
            node = NULL;
            hashTable->size = hashTable->size - 1;
            return;
        }

        previous = node;
        node = node->next;
    }
}

int main() {
    HashTable *table = newHashTable(10);

    printf("Hash table size: %d\n", table->size);
    printf("Hash table is empty? %d\n", isEmpty(table));

    put(table, 0, "test");
    put(table, 10, "foo");
    put(table, 7, "Patrick");
    put(table, 0, "Santos");
    put(table, 100, "world");
    put(table, 1, "a");
    put(table, 2, "b");
    put(table, 3, "c");
    put(table, 4, "d");
    put(table, 5, "e");
    put(table, 6, "f");
    put(table, 8, "g");
    put(table, 9, "h");
    put(table, 11, "i");
    put(table, 12, "j"); 
    put(table, 19, "k");

    display(table);

    printf("Hash table size: %d\n", table->size);
    printf("Hash table is empty? %d\n", isEmpty(table));

    Node *output = get(table, 0);

    delete(table, 9);
    display(table);
}
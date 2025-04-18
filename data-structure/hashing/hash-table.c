#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int key;
    char *value;
    struct Node *next;
} Node;

typedef struct {
    Node *buckets;
    int capacity;
    int size;
} HashTable;

HashTable* newHashTable(int capacity) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));

    if(table == NULL) return NULL;

    table->capacity = capacity;
    table->size = 0;
    table->buckets = (Node *)malloc(capacity * sizeof(Node));

    if(table->buckets == NULL) {
        free(table);

        return NULL;
    }
     
    for(int i = 0; i < capacity; i++) {
        table->buckets[i].key = 0;
        table->buckets[i].value = NULL;
        table->buckets[i].next = &(table->buckets[i + 1]);
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
    Node *temp = &(hashTable->buckets[index]);

    if(temp->value == NULL) {
        hashTable->buckets[index] = *node;
        hashTable->size++;
    } else {
        while(temp->value != NULL && temp->next != NULL) {
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
        node = &table->buckets[i];

        if(node->value == NULL) continue;

        printf("%d ->", i);

        while(node != NULL) {
            printf(" [%d, %s]", node->key, node->value);
            node = (Node *)node->next;
        }
        printf("\n");
    }
}

// char* get(HashTable *HashTable, int key) {
//     return NULL;
// }

// char* remove(HashTable *HashTable, int key) {

// }

int main() {
    HashTable *table = newHashTable(10);

    printf("Hash table size: %d\n", table->size);
    printf("Hash table is empty? %d\n", isEmpty(table));

    put(table, 0, "test");
    put(table, 10, "foo");
    put(table, 7, "Patrick");
    put(table, 0, "Santos");
    put(table, 100, "world");

    display(table);

    printf("Hash table size: %d\n", table->size);
    printf("Hash table is empty? %d\n", isEmpty(table));
}
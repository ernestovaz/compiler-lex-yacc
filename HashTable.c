#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable.h"

HashTable* createTable(){
    HashTable* hashtable = malloc(sizeof(HashTable));
    hashtable->size = TABLE_SIZE;
    hashtable->count = 0;
    hashtable->table = calloc(hashtable->size,sizeof(HashTableNode*));
    for(int i=0; i<hashtable->size; i++){
        hashtable->table[i] = NULL;
    }
    return hashtable;
}

void insert(HashTable* hashtable, char* key, int value){
    HashTableNode* node = createNode(key, value);

    int index = hashFunction(key);
    HashTableNode* position = hashtable->table[index];
    if (position == NULL){
        if (hashtable->count == hashtable->size){
            printf("Error: Hash Table is full\n");
            return;
        }
        hashtable->table[index] = node;
        hashtable->count++;
    }
    else{
        handleCollision(position, node);
    }

}

int search(HashTable* hashtable, char* key){
    int index = hashFunction(key);
    HashTableNode* node = hashtable->table[index];

    while(node != NULL){
        if (strcmp(node->key, key) == 0){
            return node->value;
        } else if (node->next == NULL){
            return -1;
        }
        node = node->next;
    }

    return -1;
}

void freeTable(HashTable* hashTable) {
    HashTableNode *node, *next;
    for(int i=0; i<hashTable->size; i++) {
        node = hashTable->table[i];
        while(node != NULL) {
            next = node->next;
            free(node);
            node = next;
        }
    }
    free(hashTable);
}

void printSearch(HashTable* hashTable, char* key){
    const int value = search(hashTable, key);
    if (value == -1){
        printf("%s does not exist\n", key);
        return;
    } else{
        printf("Key:%s, Value:%d", key, value);
    }
}

void printTable(HashTable* hashTable){
    HashTableNode* node;
    printf("\n-------------------\n");
    for (int i=0; i<hashTable->size; i++) {
        node = hashTable->table[i];
        if (node != NULL) {
            printf("Index:%d, Key:%s, Value:%d", 
                    i, 
                    hashTable->table[i]->key, 
                    hashTable->table[i]->value);
            node = node->next;
            while(node != NULL) {
                printf(" => Key:%s, Value:%d ", 
                        node->key, 
                        node->value);
                node = node->next;
            }
            printf("\n");
        }
    }
    printf("-------------------\n");
}


int hashFunction(char* key){
    int sum = 0;
    for (int i=0; i<strlen(key); i++){
        sum = (3*sum+(int)key[i]) % TABLE_SIZE;
    }
    return sum;
}

HashTableNode* createNode(char* key, int value) {
    HashTableNode* node = malloc(sizeof(HashTableNode));
    node->key = (char*) malloc (strlen(key) + 1);
    node->value = value;
    strcpy(node->key, key);
    return node;
}

void handleCollision(HashTableNode* position, HashTableNode* node){
    while(position != NULL){
        if (strcmp(position->key, node->key) == 0){
            position->value = node->value;
            return;
        } 
        if (position->next == NULL) {
            position->next = node;
            return;
        }
        position = position->next;
    }
}

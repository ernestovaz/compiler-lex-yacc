#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

LinkedList** createOverflowBuckets(HashTable* table) {
    // Create the overflow buckets; an array of linkedlists
    LinkedList** buckets = (LinkedList**) calloc (table->size, sizeof(LinkedList*));
    for (int i=0; i<table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

HashTable* createTable(){
    HashTable* hashtable = malloc(sizeof(HashTable));
    hashtable->size = TABLESIZE;
    hashtable->count = 0;
    hashtable->table = calloc(hashtable->size, sizeof(Item*));
    for(int i=0; i<hashtable->size; i++){
        hashtable->table[i] = NULL;
    }
    hashtable->overflowBuckets = createOverflowBuckets(hashtable);

    return hashtable;
}

int hashFunction(char* key){
    int sum = 0;

    for (int i=0; i<strlen(key); i++){
        sum = (3*sum+(int)key[i]) % TABLESIZE;
    }

    
    return sum;
}

void handleCollision(HashTable* hashtable, int index, Item* item){
    LinkedList* head = hashtable->overflowBuckets[index];
    
    if (head == NULL){
        head = allocateList();
        head->item = item;
        hashtable->overflowBuckets[index] = head;
        return;
    }
    else{
        //hashtable->overflowBuckets[index] = linkedlistInsert(head, item);
        linkedlistInsert(head,item);
        return;
    }
}

void insert(HashTable* hashtable, char* key, int data){
    Item* newItem = (Item*) malloc(sizeof(Item));
    newItem->key = (char*) malloc(sizeof(key));
    strcpy(newItem->key, key);
    newItem->value = data;

    int index = hashFunction(key);
    
    Item* item = hashtable->table[index];
    if (item == NULL){
        if (hashtable->count == hashtable->size){
            printf("Error: Hash Table is full\n");
            return;
        }
        hashtable->table[index] = newItem;
        hashtable->count++;
    }
    else{
        if (strcmp(item->key, key) == 0){
            hashtable->table[index]->value = data;
            return;
        }
        else{
            handleCollision(hashtable, index, newItem);
            return;
        }
    }

}

int search(HashTable* hashtable, char* key){
    int index = hashFunction(key);
    Item* item = hashtable->table[index];
    LinkedList* head = hashtable->overflowBuckets[index];

    while(item != NULL){
        if (strcmp(item->key, key) == 0){
            return item->value;
        }
        if (head == NULL){
            return -1;
        }
        item = head->item;
        head = head->next;
    }
    return -1;
}

void printSearch(HashTable* hashtable, char* key){
    int val;
    if ((val = search(hashtable, key)) == -1){
        printf("%s does not exist\n", key);
        return;
    }
    else{
        printf("Key:%s, Value:%d", key, val);
    }
}

void printTable(HashTable* hashtable){
    printf("\n-------------------\n");
    for (int i=0; i<hashtable->size; i++) {
        if (hashtable->table[i]) {
            printf("Index:%d, Key:%s, Value:%d", i, hashtable->table[i]->key, hashtable->table[i]->value);
            if (hashtable->overflowBuckets[i]) {
                printf(" => Overflow Bucket => ");
                LinkedList* head = hashtable->overflowBuckets[i];
                while (head) {
                    printf("Key:%s, Value:%d ", head->item->key, head->item->value);
                    head = head->next;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------\n");
}

int main(){
    HashTable* hashtable = createTable();
    insert(hashtable, "palavra", 1);
    insert(hashtable, "segunda", 2);
    insert(hashtable, "segunda", 2);
    insert(hashtable, "naosei", 4);
    insert(hashtable, "naosei", 5);
    insert(hashtable, "naosei", 6);

    printTable(hashtable);
    return 0;
}

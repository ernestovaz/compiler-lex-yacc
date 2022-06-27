#define TABLESIZE 997
#include "linkedlist.h"

struct hashtable{
    int size;
    int count;
    Item** table;
    LinkedList** overflowBuckets;
};

typedef struct hashtable HashTable;

int hashFunction(char* key);
LinkedList** createOverflowBuckets(HashTable* table);
HashTable* createTable();
void handleCollision(HashTable* hashtable, int index, Item* item);
void insert(HashTable* hashtable, char* key, int data);
int search(HashTable* hashtable, char* key);
void printSearch(HashTable* hashtable, char* key);
void printTable(HashTable* hashtable);

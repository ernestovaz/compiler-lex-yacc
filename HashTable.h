#ifndef HASH_TABLE
#define HASH_TABLE

#define TABLE_SIZE 997

typedef struct hash_table_node {
    char* key;              
    int value;
    struct hash_table_node* next;
} HashTableNode;

typedef struct hash_table {
    int size;
    int count;
    HashTableNode** table;
} HashTable;

// External functions
HashTable* createTable();
void insert(HashTable* hashtable, char* key, int value);
int search(HashTable* hashtable, char* key);
void freeTable(HashTable* hashTable);
void printSearch(HashTable* hashtable,  char* key);
void printTable(HashTable* hashtable);

//Internal functions
int hashFunction( char* key);
HashTableNode* createNode(char* key, int value);
void handleCollision(HashTableNode* position, HashTableNode* node);

#endif

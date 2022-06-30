#ifndef HASH_TABLE
#define HASH_TABLE

#define TABLE_SIZE 997

typedef struct hash_table_node {
    const char* key;              
    const char* value;
    struct hash_table_node* next;
} HashTableNode;

typedef struct hash_table {
    int size;
    int count;
    HashTableNode** table;
} HashTable;

// External functions
HashTable* createTable();
void insert(HashTable* hashtable, const char* key, const char* value);
const char* search(HashTable* hashtable, const char* key);
void freeTable(HashTable* hashTable);
void printSearch(HashTable* hashtable, const char* key);
void printTable(HashTable* hashtable);

//Internal functions
int hashFunction(const char* key);
HashTableNode* createNode(const char* key, const char* value);
void handleCollision(HashTableNode* position, HashTableNode* node);

#endif

#define TABLESIZE 997

struct item{
    char* key;
    int value;
};

typedef struct item Item;

struct hashtable{
    char* table[TABLESIZE];
};

typedef struct hashtable HashTable;

int hashFunction(char* key);
void insert(HashTable* hashtable, char* key);
int search(HashTable* hashtable, char* key);

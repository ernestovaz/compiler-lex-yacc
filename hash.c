#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

int hashFunction(char* key){
    int sum = 0;

    for (int i=0; i<strlen(key); i++){
        sum = 3*sum+(int)key[i];
    }
    
    return sum%TABLESIZE;
}

void insert(HashTable* hashtable, char* key){
    int index = hashFunction(key);
    char* element = malloc(sizeof(key));

    hashtable->table[index] = element;

}

int search(HashTable* hashtable, char* key){
    int index = hashFunction(key);
    while(hashtable->table[index] != NULL){
        //if 
    }
    return 0;
}

int main(){
    HashTable hashtable;
    char hello[] = "Hello World";
    insert(&hashtable, hello);

    puts(hello);
    return 0;
}
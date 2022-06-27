struct item{
    char* key;
    int value;
};

typedef struct item Item;

typedef struct LinkedList LinkedList;
 
struct LinkedList {
    Item* item; 
    LinkedList* next;
};

LinkedList* allocateList ();
LinkedList* linkedlistInsert(LinkedList* list, Item* item);
Item* linkedlistRemove(LinkedList* list);
void freeLinkedlist(LinkedList* list);
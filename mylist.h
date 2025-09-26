#ifndef _MYLIST_
#define _MYLIST_
#include <stdbool.h>

struct LinkedListNode{
    //Node : value poitner and next pointer to next node struct
    void* value;
    struct LinkedListNode* next;
};
typedef struct LinkedListNode LinkedListNode;


struct LinkedList{
    LinkedListNode* head;
};
typedef struct LinkedList LinkedList;

// struct LinkedListIterator {
//    LinkedListNode* next;
// };
// typedef struct LinkedListIterator LinkedListIterator;

/*
* Allocates memory for the list 
* Return a pointer to the head of new list
*/
LinkedList* list_create();
/*
 Deallocate memory for a list allocated on the heap space
*/
void list_destroy(LinkedList* list);

void list_init(LinkedList* list);
void list_deinit(LinkedList* list);

bool list_contains(LinkedList* list, void* val);
bool list_remove(LinkedList* list, void* val);
bool list_insert(LinkedList* list, void* val);
/* Remove the 1st node in the linkedlist
Does not free any mmeory associated with the popped node
*/
void* list_pop(LinkedList* list);

int list_size(LinkedList* list);
bool list_empty(LinkedList* list);

//LinkedListIterator list_iter(LinkedList* list);
//LinkedListNode * list_iter_next(LinkedListIterator *iter);


#endif // _MYLIST_

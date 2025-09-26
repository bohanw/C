#include "mylist.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Following API defined
LinkedList* list_create();

void list_destroy(LinkedList* list);

void list_init(LinkedList* list);

void list_deinit(LinkedList* list);

bool list_contains(LinkedList* list, void* val);
bool list_remove(LinkedList* list, void* val);

bool list_insert(LinkedList* list, void* val);

int list_size(LinkedList* list);
bool list_empty(LinkedList* list);

LinkedListIterator list_iter(LinkedList* list);
LinkedListNode * list_iter_next(LinkedListIterator *iter);
*/
LinkedList* list_create(){
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list_init(list);
    return list;
}

void list_init(LinkedList* list){
    list->head = NULL;
}

void list_destroy(LinkedList* list){
    if(list != NULL){
        list_deinit(list);
        free(list);
    }
}
void list_deinit(LinkedList* list){
    while(!list_empty(list)){
        list_pop(list);
    }
}

bool list_contains(LinkedList* list, void* value){
    //LinkedListIterator iter = list_iter(list);
    LinkedListNode* node = list->head;

    while((node != NULL)){
        if(node->value == value){
            return true;
        }
        node = node->next;
    }
    return false;
}
bool list_insert(LinkedList* list, void* value){
    LinkedListNode* node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if(node != NULL){
        node->value = value;
        node->next = NULL;
        if(list_empty(list)){
            list->head = node;
        }
        else {
            //iterate to the end of list, and append the new node at the end
            LinkedListNode* curr = list->head;
            while(curr != NULL && curr->next != NULL){
                curr = curr->next;

            }
            if(curr != NULL){
                curr->next= node;
            }
        }
        return true;

    }
    return false;
}

bool list_remove(LinkedList * list, void * value)
{
    LinkedListNode* prev = NULL;
    LinkedListNode* nxt = list->head;
    while(nxt !=NULL){
        if(nxt->value == value){
            if(prev == NULL){
                list->head = nxt->next;
            }
            else {
                prev->next = nxt->next;
            }
            free(nxt);
            return true;
        }
        prev = nxt;
        nxt = nxt -> next;
    }
    return false;
}


void* list_pop(LinkedList* list){
    LinkedListNode* node = list->head;
    //update new head as the current head's  nxt node
    if(list->head != NULL){
        list->head = list->head->next;
    }    

    //cache the value to be returned 
    void* val = node->value;

    //free memory of the ndoe after malloc call
    free(node);
    return val;
}

int  list_size(LinkedList* list){
    int size = 0;
    LinkedListNode* node = list->head;
    while(node != NULL){
        ++size;
        node = node->next;
    }
    return size;
}

bool list_empty(LinkedList* list){
    return list_size(list) == 0;
}



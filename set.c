#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"


typedef struct setnode{
    const char* key;
    void* item;
    struct setnode* next;

} setnode_t;


typedef struct set{
    struct setnode* head;
} set_t;

static setnode_t * setnode_new(const char* key, void* item){
    setnode_t* new_node = malloc(sizeof(setnode_t));
    if(new_node == NULL){
        return NULL;
    }
    else {
        new_node -> key = key;
        new_node -> item = item;
        new_node->next = NULL;
        return new_node;
    }
}
set_t* set_new(void){
    set_t* set = malloc(sizeof(set_t));
    if(set == NULL){
        return NULL;
    }
    else {
        set->head = NULL;
        return set;
    }

}

bool* set_insert(set_t* set, const char * key, void* item)
{
    if(set != NULL && key != NULL && item != NULL){
        if(set_find(set, key) == NULL){
            char * newkey = malloc(sizeof(char)* (strlen(key) + 1));
            strcpy(newkey, key);
            setnode_t * new_node = setnode_new(newkey, item);
            new_node->next = set->head;
            set->head = new_node;
            return true;
        }
    }
    return false;
}


void* set_find(set_t* set, const char* key){
    if(set != NULL && key != NULL){
        setnode_t * curr = set-> head;
        while(curr !=NULL){
            if(strcmp(curr->key, key) == 0){ // found a match
                return curr->item;
            }
        }
    }

    return NULL;
}

void set_print(set_t *set, 
FILE *fp, 
void (*itemprint)(FILE *fp, const char *key, void *item) ){
    
}

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */

void set_iterate(set_t * set, void * args)
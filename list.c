/* Kimi Mitsunaga
 * Term Project: Loony Lists
 * June 5th, 2026
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.h"


#define maxlength 10

struct node {
    struct node* prev;
    struct node* next;
    void** data;
    int first;
    int countA;         // count of elements in array
    int length;
};

struct list {
    struct node* header;
    int countN;         // total count of items across all nodes
    int (*compare)();
};

// ------------ FUNCTIONS ------------- //

struct node* createNode(LIST* lp, struct node* prev, struct node* next) {
    struct node* new_node = malloc(sizeof(struct node));
    assert(new_node != NULL);
    
    new_node->prev = prev;
    new_node->next = next;

    int newLength = (prev->length == 0 ) ? maxlength : 2 * prev->length ;

    new_node->data = malloc( sizeof(void*) * newLength );
    new_node->length = newLength;

    if (prev != NULL){
        prev->next = new_node;
    }
    if (next != NULL){
        next->prev = new_node;
    }

    new_node->first = 0; // Better practice to start at 0 for clean arrays
    new_node->countA = 0;

    return new_node;
}

LIST* createList(void) {
    LIST *lp = malloc(sizeof(LIST));
    assert(lp != NULL);

    struct node* dummy = malloc(sizeof(struct node));
    assert(dummy != NULL);

    lp->header = dummy;
    lp->header->next = dummy;
    lp->header->prev = dummy;

    lp->header->length = 0;
    lp->header->countA = 0;
    lp->countN = 0;
    lp->compare = NULL;

    return lp;
}

void destroyList(LIST *lp) {
    assert(lp != NULL);
    struct node* curr = lp->header->next;
    struct node* temp;

    while (curr != lp->header) {
        temp = curr;
        curr = curr->next;
        free(temp->data);
        free(temp);
    }

    free(lp->header);
    free(lp);
}

int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->countN;
}
void addFirst(LIST *lp, void *item){

    assert( lp != NULL && item != NULL);
    struct node* curr;

    if(lp->countN == 0){
        curr = createNode(lp, lp->header, lp->header);
    }
    else{
        curr = lp->header->next;
    }

    if( curr->countA == curr->length){
        curr = createNode(lp, lp->header, curr);
    }
    if( curr->countA != 0){
        curr->first = (curr->first - 1 + curr->length ) % curr->length;
    }

    curr->data[curr->first] = item;
    curr->countA++;
    lp->countN++;


}
void addLast(LIST *lp, void *item){

    assert (lp != NULL && item != NULL);

    struct node *curr = lp->header->prev;
    int last;

    if (lp->countN == 0){

        curr = createNode(lp, lp->header, lp->header);
    }
    else{
        curr = lp->header->prev;
    }
    if (curr->countA == curr->length){
        curr = createNode(lp, curr, lp->header);
    }

    last = (curr->first + curr->countA) % curr->length;

    curr->data[last] = item;
    curr->countA++;
    lp->countN++;
}

void *removeFirst(LIST *lp){

    assert (lp != NULL);

    if (lp->countN == 0){
        return NULL;
    }

    struct node *temp = lp->header->next;
    struct node *curr = lp->header->next;

    void *deletedVal;

    if (curr->countA == 0) {

        temp = curr;
        curr = curr->next;
        curr->prev = lp->header;
        lp->header->next = curr;

        free(temp->data);
        free(temp);

    }

    deletedVal = curr->data[curr->first];
    //value = curr->data[curr->first];
    curr->data[curr->first] = NULL;
    curr->first = (curr->first + 1) % curr->length;
    curr->countA--;
    lp->countN--;

    return deletedVal;

}

void *removeLast(LIST *lp){

    assert (lp != NULL);

    if (lp->countN == 0){
        return NULL;
    }

    struct node *temp = lp->header->prev;
    struct node *curr = lp->header->prev;

    void *delVal;
    int last;

    if (curr->countA == 0) {

        temp = curr;
        curr = curr->prev;
        curr->next = lp->header;
        lp->header->prev = curr;

        free(temp->data);
        free(temp);
    }

    last = (curr->first + curr->countA - 1) % curr->length;

    delVal = curr->data[last];
    //value = curr->data[last];
    curr->data[last] = NULL;
    curr->countA--;
    lp->countN--;

    return delVal;

}

void *getItem(LIST *lp, int index){

    if (lp->countN == 0) {

        printf("List is empty, no elements\n");
        return NULL;
    }

    else if (index >= lp->countN || index < 0) {

        printf("Index is invalid\n");
        return NULL;
    }

    else {

        struct node *curr = lp->header->next;

        while (index - curr->length >= 0 && curr->length != 0) {

            index -= curr->length;
            curr = curr->next;
        }
        return curr->data[(curr->first + index) % curr->length];
    }
}

void setItem( LIST *lp, int index, void*item){

    if(lp->countN == 0){
        printf("No nodes in list\n");
    }
    else if( (index >= lp->countN) || (index < 0 ) ){
        printf("Index is invalid\n");
    }
    else{
        struct node* curr = lp->header->next;
        while( (index - curr->countA >= 0 ) && (curr->length != 0) ){
            index -= curr->countA;
            curr = curr->next;
        }

        curr->data[ (curr->first + index) % curr->length ] = item;
    }


}


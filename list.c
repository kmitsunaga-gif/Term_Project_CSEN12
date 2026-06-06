/* Kimi Mitsunaga
 * Term Project: Loony Lists
 * June 5th, 2026
 *
 * Project Description:
 *
 *   
 *   
 */

#include <stdio.h>
#include<assert.h>
#include <stdlib.h>

#define maxlength 10

struct node{

    struct node* prev;
    struct node* next;

    void** data;
    int first;

    int countA;         //count of elements in array
    int length;

};

struct list{

    struct node* header;
    int countN;             //count of nodes in list
    int (*compare)();

};


// ------------ FUNCTIONS ------------- //

//Purpose: creates new node
//Parameters: left and right nodes, count,
//Returns: new node pointer
//Runtime: O(1)

struct node* createNode( LIST* lp, struct node* prev, struct node* next){ // p = prev, n = next

    assert( count >= 0 );

    struct node* new = malloc( sizeof( struct node ) );

    new->data = malloc( sizeof(struct node) * 2 * maxlength );
    new->length = length * 2;
    
    new->prev = prev;
    new->next = next;

    prev->next = new;
    next->prev = new;

    new->first = 5;
    new->countA = 0;


    return new;

}

/* Purpose: creates new node
// Parameters: function pointer compare()
// Returns: new list pointer
// Runtime: O(1)
*/

LIST* createList( int (*compare)() ){

    LIST *lp = malloc( sizeof( LIST) );

    node* dummy = malloc( sizeof( struct node ) );

    lp->header = dummy;
    lp->header->next = dummy;
    lp->header->prev = dummy;

    lp->header->length = 0;
    lp->header->countA = 0;

    lp->compare = compare;

    return lp;

}

void destroyList(LIST *lp){
    
    struct node* temp;

    while(lp->header->next != lp->header){
        temp = lp->header->next;
        lp->header->next = temp->next;
        free(temp->data);
        free(temp);
    }

    free(lp->header);
    free(temp);

}

int numItems(LIST *lp){

    assert( lp != NULL);
    return lp->countN;

}

void addFirst(LIST *lp, void *item){

    assert( lp != NULL && item != NULL);
    struct node curr;

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


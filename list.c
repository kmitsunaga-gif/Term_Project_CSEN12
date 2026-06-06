/* Kimi Mitsunaga
 * Term Project: Loony Lists
 * June 5th, 2026
 *
 * Loony List Lab:
 *      This lab deals with a hybrid data structure. 
 *      Our structure of this lab is comprised of a doubly 
 *      circular linked list, with only a header node, 
 *      and each node holding a circular queue array 
 *      within each node. Also note that each node's array 
 *      is doubled when one has been filled. There are 
 *      12 functions coded to traverse, add, delete, and 
 *      retrieve elements in our data structre.  
 *
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

/*Purpose: creating a new node
//Parameters: list pointer, previous node, and next node
//Returns: new node
//Runtime: O(1)
*/

struct node* createNode(LIST* lp, struct node* prev, struct node* next) {

    //make space for new node!
    struct node* new = malloc(sizeof(struct node));
    assert(new != NULL);
    
    //connect new node to given nodes
    new->prev = prev;
    new->next = next;

    //calculate length
    int newLength = (prev->length == 0 ) ? maxlength : 2 * prev->length ;

    //make space for array, 2x the size of the last node's length
    new->data = malloc( sizeof(void*) * newLength );
    new->length = newLength;

    if (prev != NULL){
        //if no prev, punt new node behind next node
        prev->next = new;
    }
    if (next != NULL){
        //if no next node, put new node in front of prev node
        next->prev = new;
    }

    new->first = 0; // Better practice to start at 0 for clean arrays
    new->countA = 0;

    return new;
}


/*Purpose: creates linked list
//Parameters: nothing
//Returns: list pointer
//Runtime: O(1)
*/

LIST* createList(void) {

    //make space for list!
    LIST *lp = malloc(sizeof(LIST));
    assert(lp != NULL);

    //make space for header node
    struct node* dummy = malloc(sizeof(struct node));
    assert(dummy != NULL);

    //assign relationships for header node
    lp->header = dummy;
    lp->header->next = dummy;
    lp->header->prev = dummy;

    //assign length, count, and compare vals
    lp->header->length = 0;
    lp->header->countA = 0;
    lp->countN = 0;
    lp->compare = NULL;

    return lp;
}


/*Purpose: destories LL
//Parameters: LL pointer
//Returns: nothing
//Runtime: O(n) ; n = number of nodes in LL
*/

void destroyList(LIST *lp) {

    assert(lp != NULL);

    //initalize ptr vars
    struct node* curr = lp->header->next;
    struct node* temp;

    while (curr != lp->header) {
        //traverse through each node and delete
        //move onto next node

        temp = curr;
        curr = curr->next;

        free(temp->data);
        free(temp);
    }

    //free header and the LL itself
    free(lp->header);
    free(lp);
}

/*Purpose: gets number of nodes in LL
//Parameters: LL pointer
//Returns: number of nodes
//Runtime: O(1)
*/

int numItems(LIST *lp) {

    assert(lp != NULL);
    return lp->countN;

}

/*Purpose: Adds element into begining of LL
//Parameters: LL ptr, element being added
//Returns: nothing
//Runtime: O(1) 
*/

void addFirst(LIST *lp, void *item){

    assert( lp != NULL && item != NULL);
    struct node* curr;

    if(lp->countN == 0){
        //if LL enpty
        curr = createNode(lp, lp->header, lp->header);
    }
    else{
        //LL holds nodes
        curr = lp->header->next;
    }

    if( curr->countA == curr->length){
        //checks if array (element) count is equal to array length
        curr = createNode(lp, lp->header, curr);
    }
    if( curr->countA != 0){
        //as long as array holds some element
        curr->first = (curr->first - 1 + curr->length ) % curr->length;
    }

    //add element
    curr->data[curr->first] = item;
    curr->countA++;
    lp->countN++;


}

/*Purpose: adds element into ending of LL
//Parameters: LL ptr, element being added
//Returns: nothing 
//Runtime: O(1)
*/

void addLast(LIST *lp, void *item){

    assert (lp != NULL && item != NULL);

    struct node *curr = lp->header->prev;
    int last;

    if (lp->countN == 0){
        //if LL empty

        curr = createNode(lp, lp->header, lp->header);
    }
    else{
        //LL is not empty
        curr = lp->header->prev;
    }
    if (curr->countA == curr->length){
        // if node array count == length
        curr = createNode(lp, curr, lp->header);
    }

    last = (curr->first + curr->countA) % curr->length;

    //add element
    curr->data[last] = item;
    curr->countA++;
    lp->countN++;
}


/*Purpose:  removes first element in LL
//Parameters: LL ptr
//Returns: nothing
//Runtime: O(1)
*/


void *removeFirst(LIST *lp){

    assert (lp != NULL);

    if (lp->countN == 0){
        return NULL;
    }

    //initalize vars
    struct node *temp = lp->header->next;
    struct node *curr = lp->header->next;

    void *deletedVal;

    if (curr->countA == 0) {
        // when array in node empty

        temp = curr;
        curr = curr->next;
        curr->prev = lp->header;
        lp->header->next = curr;

        free(temp->data);
        free(temp);

    }

    // hold onto value
    deletedVal = curr->data[curr->first];
    //set slot of deleted val to NULL
    curr->data[curr->first] = NULL;
    //first item is now next item
    curr->first = (curr->first + 1) % curr->length;
    //decrement count
    curr->countA--;
    lp->countN--;

    return deletedVal;

}

/*Purpose:  removes last element in LL
//Parameters: LL ptr
//Returns: nothing
//Runtime: O(1)
*/

void *removeLast(LIST *lp){

    assert (lp != NULL);

    if (lp->countN == 0){
        return NULL;
    }

    // initalize vars
    struct node *temp = lp->header->prev;
    struct node *curr = lp->header->prev;

    void *delVal;
    int last;

    if (curr->countA == 0) {
        // if node arr empty

        temp = curr;
        curr = curr->prev;
        curr->next = lp->header;
        lp->header->prev = curr;

        free(temp->data);
        free(temp);
    }

    //reassign last
    last = (curr->first + curr->countA - 1) % curr->length;
    // save deleted data, set it's slot to NULL
    delVal = curr->data[last];
    curr->data[last] = NULL;
    //decr count
    curr->countA--;
    lp->countN--;

    return delVal;

}

/*Purpose: retrieves item! 
//Parameters: LL ptr, index of item
//Returns: nothing
//Runtime: O( log(n) ) - our arrays are doubled in size each 
//                      time we make a new node, so as we search 
//                      we are able to jump over each node rather 
//                      than look at every single void pointer for our item. 
//                      
*/

void *getItem(LIST *lp, int index){

    if (lp->countN == 0) {
        //make sure list is not empty

        printf("List is empty, no elements\n");
        return NULL;
    }

    else if ( (index >= lp->countN) || (index < 0)) {
        //make sure index is valid

        printf("Index is invalid\n");
        return NULL;
    }

    else {

        struct node *curr = lp->header->next;

        while (index - curr->length >= 0 && curr->length != 0) {
            //traverse LL

            //subtract current node's arr length, we jump to next node
            index -= curr->length;
            curr = curr->next;
        }

        return curr->data[ (curr->first + index) % curr->length];
    }
}


/*Purpose: assigns item to index in LL
//Parameters: LL ptr, index for insertion, item being inserted
//Returns: nothing
//Runtime: O( log(n) ) - similarly to getItem, we traverse 
//                       through LL the same, by jumping from 
//                       node to node, so our runtime is still O( log(n) );
*/

void setItem( LIST *lp, int index, void*item){

    if(lp->countN == 0){
        //make sure list not empty
        printf("No nodes in list\n");
    }
    else if( (index >= lp->countN) || (index < 0 ) ){
        //make sure index valid

        printf("Index is invalid\n");
    }
    else{

        struct node* curr = lp->header->next;

        while( (index - curr->countA >= 0 ) && (curr->length != 0) ){
            //traverse lLL

            //minus out num of elements in arr
            index -= curr->countA;
            //jump to next node
            curr = curr->next;
        }
        //assign item to desired index
        curr->data[ (curr->first + index) % curr->length ] = item;
    }


}


// PQ.c 
// taken from week12 lecture slides
/* written by 
        Haoyue Qiu(z5123104@unsw.edu.au)               */
    
#include<stdio.h>
#include<stdlib.h>
#include <assert.h>
#include "PQ.h"

#define MaxSlots 2048 
#define True 1
#define False 0

typedef struct ItemPQ *Item;   
//heap
typedef struct PQRep{
    Item* items;
    int nitems;
    int nslots;
}PQRep;

int exist = 0; 
// to find if the key has already existed in PQ
void swap(PQ pq,int a,int b);
// to swap PQ->items[a] and PQ->items[b]
int checkOrSwap(PQ pq,int root,int left,int right);
//return the index of the child(left or right) swap with,or return false for not swap.


PQ newPQ() {
    PQ new = malloc(sizeof(PQRep));
    assert(new != NULL);
    new->nslots = MaxSlots;
    new->nitems = 0;
    new->items  = malloc((MaxSlots+1)*sizeof(Item));
    // initialize each value within PQ
    int i = 0;
    while(i <= MaxSlots){
        new->items[i] = malloc(sizeof(PQRep));
        // allocate memory to items[] to be big enough to store all the paths
        i++;
    }
    return new;
}


void addPQ(PQ pq, ItemPQ im){
    if (exist == 1) {
    // if the key was in PQ then just update the new value of the key
        updatePQ(pq,im);
        return;    
    }
    pq->nitems++;  
    *pq->items[pq->nitems] = im;
    // add new key into PQ
    int n = pq->nitems;
    while (n > 1 && pq->items[n/2]->value > pq->items[n]->value) {
        // if there is more than one key and its value is smaller than the middle node
        swap(pq, n, n/2);
        // keep swapping until the key with smallest is put as first node
        n = n/2;      
    }
}


void swap(PQ pq, int a,int b){

    Item c = pq->items[a];
    pq->items[a] = pq->items[b];
    pq->items[b] = c;
    // swap a and b
}


ItemPQ dequeuePQ(PQ pq){
    swap(pq,1, pq->nitems);
    // swap the first node and the last node
    pq->nitems--;
    // decrease the number of nodes in PQ
    int n = 1;
    while(2*n <= pq->nitems){
    // to reorder PQ
        if((2*n+1) == pq->nitems+1){
        // if n = nitems/2 which means the last two nodes may need to be swapped
            if(pq->items[2*n]->value < pq->items[n]->value) swap(pq, n, 2*n);
            break;
        }
        //return the index of the child(left or right) swap with,or return false for not swap.
        n = checkOrSwap(pq,n,2*n,2*n+1);
        if(n == False)  break;
    }
    return *pq->items[pq->nitems+1];
}


int checkOrSwap(PQ pq,int root,int left,int right){
    
    if(pq->items[root]->value < pq->items[left]->value && pq->items[root]->value < pq->items[right]->value){
    // if root is smaller than both right and left child
        return False;
    // no need to swap
    }else if(pq->items[root]->value > pq->items[left]->value && pq->items[root]->value < pq->items[right]->value){
    // if right > root > left
        swap(pq,root,left);
        return left;
    }else if(pq->items[root]->value < pq->items[left]->value && pq->items[root]->value > pq->items[right]->value){
    // if left > root > right
        swap(pq,root,right);
        return right;
    }else if(pq->items[root]->value > pq->items[left]->value && pq->items[root]->value > pq->items[right]->value){
    // if root is the biggest one then find the smallest node between left and right
        if(pq->items[left]->value < pq->items[right]->value){
            swap(pq,root,left);
            return left;
        }else{
            swap(pq,root,right);
            return right;
        }
    }
    return 0;
}


void  updatePQ(PQ pq, ItemPQ im){
    int n = 1;
    while(n <= pq->nitems){
        if(pq->items[n]->key == im.key){
        // if key alrady existed in PQ
            pq->items[n]->value = im.value;
            break;
        }
        n++;
    }
    exist = 1;
}

int PQEmpty(PQ pq){
    if(pq->nitems == 0){
        return True;
    }
    return False;
}


//is it the right way to show??? like a tree???
void  showPQ(PQ pq){
//    int n = 1;
//    int record = 1;
//    while(n <= pq->nitems){
//        printf("%d",pq->items[n]->value);
//        n++;
//        if(n == record * 2){
//            printf("\n");
//            record = record * 2;
//        }
//    }
    printf("a\n");
}

void  freePQ(PQ pq){
    int n = 1;
    while(n <= pq->nitems){
        free(pq->items[n]);
        n++;
    }
    free(pq->items);
    free(pq);
}

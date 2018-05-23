#include<stdio.h>
#include<stdlib.h>
#include "PQ.h"

#define MaxSlots 2048 //big enough?
#define True 1
#define False 0


typedef struct ItemPQ *Item;   //Is the order right?? write it here is right?

//heap
typedef struct PQRep{
    Item *items;
    int nitems;
    int nslots;
}PQRep;


void swap(Item a,Item b);
int checkOrSwap(Item a,Item b,Item c,int n);

PQ newPQ(){
    PQ new = calloc(1,sizeof(PQRep));
    new->nslots = MaxSlots;
    new->nitems = 0;
    new->items = calloc((MaxSlots+1),sizeof(Item));
    
    return new;
}

void addPQ(PQ pq, ItemPQ im){
    pq->nitems = pq->nitems+1;
    // Is that right???  &&&????????
    pq->items[pq->nitems] = &im;
    int n = pq->nitems;
    while(n/2 >= 1){
        if(pq->items[n]->value < pq->items[n/2]->value){
            swap(pq->items[n],pq->items[n/2]);
        }else{
            break;
        }
        n = n/2;
    }
}

void swap(Item a,Item b){
    Item c = a;
    a = b;
    b = c;
}

ItemPQ dequeuePQ(PQ pq){
  
    //will/Must not be empty?????
    /*  if(pq->items[1] == NULL){
        return NULL;
    }
   */
   
   //Is that right??
    Item topOne = pq->items[1];
    free(pq->items[1]);
    //move the most bottom-right one to the top
    pq->items[1] = pq->items[pq->nitems];
    pq->items[pq->nitems] = NULL;
    pq->nitems = pq->nitems-1;
    
    //fix
    int n = 1;
    while(2*n < pq->nitems){
        //return the index of the child(left or right) swap with,or return false for not swap
        n = checkOrSwap(pq->items[n],pq->items[2*n],pq->items[2*n+1],n);
        if(n == False){
            break;
        }
    }
    return *topOne;
}

int checkOrSwap(Item a,Item b,Item c,int n){
    if(c != NULL){
        if(a->value < b->value && a->value < c->value){
            return False;
        }else if(a->value > b->value && a->value < c->value){
            swap(a,b);
            return 2*n;
        }else if(a->value < b->value && a->value > c->value){
            swap(a,c);
            return 2*n+1;
        }else if(a->value > b->value && a->value > c->value){
            if(b->value < c->value){
                swap(a,b);
                return 2*n;
            }else{
                swap(a,c);
                return 2*n+1;
            }
        }
    // for the case: c == NULL
    }else{
        if(a->value < b->value){
            return False;
        }else{
            swap(a,b);
            return 2*n;
        }
    }
    return 0;
}

void  updatePQ(PQ pq, ItemPQ im){
    int n = 1;
    while(n <= pq->nitems){
        if(pq->items[n]->key == im.key){
            pq->items[n]->value = im.value;
            break;
        }
        n++;
    }
}

int PQEmpty(PQ pq){
    if(pq->items[1] == NULL){
        return True;
    }else{
        return False;
    }
}

//is it the right way to show??? like a tree???
void  showPQ(PQ pq){
    int n = 1;
    int record = 1;
    while(n <= pq->nitems){
        printf("%d",pq->items[n]->value);
        n++;
        if(n == record * 2){
            printf("\n");
            record = record * 2;
        }
    }
}

void  freePQ(PQ pq){
    int n = 1;
    while(n <= pq->nitems){
        free(pq->items[n]);
        n++;
    }
    
    //need to free items array??????
    free(pq->items);
    
    free(pq);
}


























#include<stdio.h>
#include<stdlib.h>
#include <assert.h>
#include "PQ.h"

#define MaxSlots 2048 //big enough?
#define True 1
#define False 0

typedef struct ItemPQ *Item;   //Is the order right?? yes!
//heap
typedef struct PQRep{
    Item items;
    int nitems;
    int nslots;
}PQRep;


void swap(PQ pq, Item a,Item b);
int checkOrSwap(Item a,Item b,Item c,int n);
PQ newPQ() {
    pQRep new = calloc(1, sizeof(pQRep));
    assert(new != NULL);
    new->nslots = MaxSlots;
    new->nitems = 0;
    new->items  = calloc(MaxSlots+1, sizeof(ItemPQ));
    return new;
}

void addPQ(PQ pq, ItemPQ im){
    int exist = 0;
    assert(pq != NULL && im != NULL);
    for (int x = 0; x < pg->nitems ; x++) {
        if (pg->items[x]->key == im->key) {
            pg->items[x]->value = im->value;
            exist = 1;
            break;
        }
    }
    if (exist == 0) {
        pq->items[pq->nitems] = &im;
        pq->nitems++;    
    }
    int n = pq->nitems;
    while (n > 1 && pq->items[n/2] < pq->items[n]) {
        swap(pq, n, k/2);
        n = n/2;
    }
}


void swap(PQ pq, Item a,Item b){

    Item c = pq->items[a];
    pq->items[a] = pq->items[b];
    pq->items[b] = c;
}

void neverViolate(ItemPQ a[], int i, int n) {
    while ( 2*i <= n) {
        int j = 2*i;
        if (j < n  && a[j].value < a[j+1].value) j++;
        if (a[i].value >= a[j]) swap(a[i], a[j]);
        i = j;
    }
}

ItemPQ dequeuePQ(PQ pq){
    if (pq != NULL && pq->nitems == 0) return NULL;
    swap(pq->items[1], pg->items[nitems]);
    pq->nitems--;
    neverViolate(pq, 1, pq->nitems);
    return pq->items[q->nitems+1];
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

























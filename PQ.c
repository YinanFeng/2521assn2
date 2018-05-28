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
    Item* items;
    int nitems;
    int nslots;
}PQRep;


void swap(PQ pq,int a,int b);
int checkOrSwap(PQ pq,int a,int b,int c);

PQ newPQ() {
    PQ new = malloc(sizeof(PQRep));
    assert(new != NULL);
    new->nslots = MaxSlots;
    new->nitems = 0;
    new->items  = malloc((MaxSlots+1)*sizeof(Item));
    int i = 0;
    while(i <= MaxSlots){
        new->items[i] = malloc(sizeof(PQRep));
        i++;
    }
    return new;
}

void addPQ(PQ pq, ItemPQ im){
    pq->nitems++;
    
    *pq->items[pq->nitems] = im;

  
    int n = pq->nitems;


    while (n > 1 && pq->items[n/2]->value > pq->items[n]->value) {
        swap(pq, n, n/2);
        n = n/2;
        
    }

}


void swap(PQ pq, int a,int b){

    Item c = pq->items[a];
    pq->items[a] = pq->items[b];
    pq->items[b] = c;
}


ItemPQ dequeuePQ(PQ pq){
   // if (pq->nitems == 0) return NULL;
  //  printf("n->items :%d\n",pq->nitems);
   // printf("de\n");
  //  printf("hereeee %d\n",pq->items[pq->nitems]->value);
  //  printf("%d\n",pq->nitems);


   // printf("print hereee");
   // printf("thereeeeee %d\n",pq->items[1]->key);
    
    swap(pq,1, pq->nitems);

    pq->nitems--;
    //printf("here\n");
    //printf("%d\n",pq->items[pq->nitems+1]->key);
    
    int n = 1;
    while(2*n <= pq->nitems){
        if((2*n+1) == pq->nitems+1){
            if(pq->items[2*n]->value < pq->items[n]->value){
                swap(pq, n, 2*n);
            }
            break;
            
        }
        //return the index of the child(left or right) swap with,or return false for not swap.
        n = checkOrSwap(pq,n,2*n,2*n+1);
        if(n == False){
            break;
        }
    }
    //printf("%d\n",pq->nitems);
   // printf("thereee\n");
 //   printf("%d\n",pq->items[pq->nitems+1]->key);
    
    
    return *pq->items[pq->nitems+1];
}


int checkOrSwap(PQ pq,int a,int b,int c){
    
    if(pq->items[a]->value < pq->items[b]->value && pq->items[a]->value < pq->items[c]->value){
        return False;
    }else if(pq->items[a]->value > pq->items[b]->value && pq->items[a]->value < pq->items[c]->value){
        swap(pq,a,b);
        return b;
    }else if(pq->items[a]->value < pq->items[b]->value && pq->items[a]->value > pq->items[c]->value){
        swap(pq,a,c);
        return c;
    }else if(pq->items[a]->value > pq->items[b]->value && pq->items[a]->value > pq->items[c]->value){
        if(pq->items[b]->value < pq->items[c]->value){
            swap(pq,a,b);
            return b;
        }else{
            swap(pq,a,c);
            return c;
        }
    }
    return 0;
}

//have not checked!!!!!!!!!!
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
    
    //need to free items array??????
    free(pq->items);
    
    free(pq);
}























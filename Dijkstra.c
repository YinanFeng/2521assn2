#include<stdio.h>
#include<stdlib.h>

#define True 1
#define False 0
#define infinity 9999999


//right??? Can I do this way?can i do it?
#include "Dijkstra.h"
#include "PQ.h"


typedef struct ShortestPaths* Shortest;



ShortestPaths dijkstra(Graph g, Vertex v){
    
    Shortest shortest = malloc(sizeof(ShortestPaths));
    shortest->noNodes = numVerticies(g);
    shortest->src = v;
    //PredNode* forcalloc;
    shortest->dist = malloc(shortest->noNodes*sizeof(int));
    shortest->pred = malloc(shortest->noNodes*sizeof(PredNode*));
    int i = 0;
    while(i < shortest->noNodes){
        shortest->dist[i] = infinity;
        shortest->pred[i] = NULL;
        i++;
    }

    int* inthePQ = malloc((shortest->noNodes)*sizeof(int));
    int h = 0;
    while(h < shortest->noNodes){
        inthePQ[h] = 0;
        h++;
    }
    
    
    PQ pq = newPQ();
    
    ItemPQ first;
    first.key = v;
    first.value = 0;
    addPQ(pq,first);
    inthePQ[first.key] = 1;
       //  temp = dequeuePQ(pq);
    
    AdjList allOut;
    shortest->dist[v] = 0;
   
    while(!PQEmpty(pq)){
      //  printf("here\n");
        ItemPQ temp;
        temp = dequeuePQ(pq);
        inthePQ[temp.key] = 0;
        
    //    printf("here");
      //  printf("%d\n",temp.key);
    
        allOut = outIncident(g,temp.key);
   //     printf("%d\n",temp.key);
  //   printf("%d\n",allOut->w);
       // printf("once\n");
        while(allOut != NULL){
          //  printf("there\n");
            PredNode* createNew = malloc(sizeof(PredNode));
            if(allOut->w != v){

            //    printf("bb\n");
              //  printf("%d\n",allOut->w);
                if(shortest->dist[allOut->w] == infinity){
                    //printf("aa\n");
                    ItemPQ newOne;
                    newOne.key = allOut->w;
                   // printf("keyyy %d\n",newOne.key);

                    
                    newOne.value = allOut->weight;
                    
              //     printf("%d\n",allOut->weight);
                 //   printf("twice\n");
                    
                   //  printf("BeforeAdd\n");
                   // showPQ(pq);
                      addPQ(pq,newOne);
                      inthePQ[allOut->w] = 1;
                      
                      
                   // printf("AfterAdd\n");
                   // showPQ(pq);
                    
                }
                //showPQ(pq);
                if(shortest->dist[temp.key] + allOut->weight < shortest->dist[allOut->w]){
                //    printf("goes here right \n");


//                    printf("%d\n",temp.value);
//                    printf("%d\n",shortest->dist[temp.value]);
                   //   PredNode* record = shortest->pred[allOut->w];
               //     PredNode* tempUse;
                    
//                    while(record != NULL){
//                        tempUse = record->next;
//                        free(record);
//                        record = tempUse;
//                    }
                   
                  
                    //printf("ggg\n");
           //         PredNode* createNew = malloc(sizeof(PredNode));
                    
                    //printf("ggg\n");
                    
                    createNew->v = temp.key;
                    
                    
                    //printf("%d",createNew->v);
            //shortest->pred[allOut->w] = malloc(sizeof(PredNode));
              
                    shortest->pred[allOut->w] = createNew;
                    shortest->dist[allOut->w] = shortest->dist[temp.key] + allOut->weight;
                    if(inthePQ[allOut->w] == 0){
                        ItemPQ theNewPQ;
                        theNewPQ.key = allOut->w;
                        theNewPQ.value = allOut->weight;
                        addPQ(pq,theNewPQ);
                        inthePQ[allOut->w] = 1;
                    }
                
                    
                    
                }else if(shortest->dist[temp.key] + allOut-> weight == shortest->dist[allOut->w]){
                    //printf("6\n");
              
                    
                    PredNode* record = shortest->pred[allOut->w];
                    //right?? record->next at first will not be record. do not need to consider record==null at first???
                    while(record->next != NULL){
                        record = record->next;
                    }
             //       PredNode* createNew = malloc(sizeof(PredNode));
                    createNew->v = temp.key;
                    record->next = createNew;
                }
            }
            allOut = allOut->next;
        }
        //mshowPQ(pq);
    }
    int l = 0;
    while(l < shortest->noNodes){
        if(shortest->dist[l] == infinity){
            shortest->dist[l] = 0;
        }
        l++;
    }
    return *shortest;
}



void  showShortestPaths(ShortestPaths sps){
    int i = 0;
    printf("Node %d\n",sps.src);
    printf("  Distance\n");
    for (i = 0; i < sps.noNodes; i++) {
        if(i == sps.src)
            printf("    %d : X\n",i);
        else
            printf("    %d : %d\n",i,sps.dist[i]);
    }
    printf("  Preds\n");
    for (i = 0; i < sps.noNodes; i++) {
        printf("    %d : ",i);
        PredNode* curr = sps.pred[i];
        while(curr!=NULL) {
            printf("[%d]->",curr->v);
            curr = curr->next;
        }
        printf("NULL\n");
    }
    
}


void  freeShortestPaths(ShortestPaths shortest){
    int i = 0;

    while(i < shortest.noNodes){
        PredNode* record = shortest.pred[i];
        PredNode* tempUse;
        while(record != NULL){
            
            tempUse = record->next;
            
            free(record);
            
            record = tempUse;
        }
        i++;
    }
    //dist[]和pred[]两个array需要free吗？ 感觉需要啊。。
    free(shortest.dist);
    free(shortest.pred);
    //??? no need to free shortest???
    //free(shortest);
}










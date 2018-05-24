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
    Shortest shortest = calloc(1,sizeof(ShortestPaths));
    shortest->noNodes = numVerticies(g);
    shortest->src = v;
    PredNode* forcalloc;
    shortest->pred = calloc(shortest->noNodes,sizeof(forcalloc));
    
    int i = 0;
    while(i < shortest->noNodes){
        shortest->dist[i] = infinity;
        shortest->pred[i] = NULL;
        i++;
    }
    
    shortest->dist[v] = 0;
    
    //key de yong chu shi shen me?
    int key = 1;
    
    PQ pq = newPQ();
    ItemPQ first;
    first.key = 1;
    key = key+1;
    first.value = v;
    addPQ(pq,first);
    
    ItemPQ temp;
    AdjList allOut;
    while(PQEmpty(pq) == False){
        //**pred is that kind of usage(pred[])
        temp = dequeuePQ(pq);
        allOut = outIncident(g,temp.value);
        while(allOut != NULL){
            while(allOut->w != v){
                if(shortest->dist[allOut->w] == infinity){
                    //right?
                    ItemPQ newOne;
                    newOne.key = key;
                    key = key+1;
                    newOne.value = allOut->w;
                    addPQ(pq,newOne);
                }
            
                if(shortest->dist[temp.value] + allOut->weight < shortest->dist[allOut->w]){
                    PredNode* record = shortest->pred[allOut->w];
                    PredNode* tempUse;
                    while(record != NULL){
                        tempUse = record->next;
                        free(record);
                        record = tempUse;
                    }
                    PredNode* createNew = calloc(1,sizeof(PredNode));
                    createNew->v = temp.value;
                    shortest->pred[allOut->w] = createNew;
                    shortest->dist[allOut->w] = shortest->dist[temp.value] + allOut->weight;
                }else if(shortest->dist[temp.value] + allOut-> weight == shortest->dist[allOut->w]){
                    PredNode* record = shortest->pred[allOut->w];
                    //right?? record->next at first will not be record. do not need to consider record==null at first???
                    while(record->next != NULL){
                        record = record->next;
                    }
                    PredNode* createNew = calloc(1,sizeof(PredNode));
                    createNew->v = temp.value;
                    record->next = createNew;
                }
            }
            allOut = allOut->next;
        }
    }
    
    
    return *shortest;
}



void  showShortestPaths(ShortestPaths shortest){
    //howwwwww toooooo showwwwwww??????
    
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










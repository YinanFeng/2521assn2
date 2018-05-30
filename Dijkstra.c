// Dijkstra.c
// taken from week07 lecture slides
/* written by
        Haoyue Qiu(z5123104@unsw.edu.au) and Yinan Feng（z5167277@unsw.edu.au*/

#include<stdio.h>
#include<stdlib.h>
#define True 1
#define False 0
#define infinity 9999999
#include "Dijkstra.h"
#include "PQ.h"


typedef struct ShortestPaths* Shortest;
Shortest new(Graph g, Vertex v);
// create new ShortestPath whose src is v
ItemPQ *newItemPQ(int key, int value);
// create new pq->items[] with given key and value
void addIntoPred(Shortest s, int nth, Vertex v);
// add new node to s->pred[]
void replacePred(Shortest s, int nth, Vertex v);
// delete all the node in pred and add a new node which the distance is shorter


Shortest new(Graph g, Vertex v) {
    Shortest new = malloc(sizeof(ShortestPaths));
    new->noNodes = numVerticies(g);
    new->src = v;
    new->dist = malloc(new->noNodes*sizeof(int));
    new->pred = malloc(new->noNodes*sizeof(PredNode*));
    // initialize each value in the node
    int i = 0;
    while(i < new->noNodes){
        new->dist[i] = infinity;
        new->pred[i] = NULL;
        i++;
    }
    return new;
}

ItemPQ *newItemPQ(int key, int value) {
    ItemPQ *new = malloc(sizeof(ItemPQ));
    new->key = key;
    new->value = value;
    // initialize all the values in ItemPQ
    return new;

}


void addIntoPred(Shortest s, int nth, Vertex v) {
    PredNode *new = malloc(sizeof(PredNode));
    new->v = v;
    new->next = NULL;
    if (s->pred[nth] == NULL) {
    // if the vertex nth hasnt been visited before;
        s->pred[nth] = new;
    } else {
    // if the vertex nth has been visited
    PredNode *curr = s->pred[nth];
    while (curr->next != NULL) {
        if (curr->v == v) return;
        // if v has already found that is the previous vertex of nth
        curr = curr->next;
    }
    if (curr->v == v) return;
    // if v has already found that is the previous vertex of nth 
    // and it is at the end of the list
    curr->next = new;
    // if v not in the list, add it to the end of list
    }
}


void replacePred(Shortest s, int nth, Vertex v) {
    while (s->pred[nth]->next != NULL) {
        PredNode *temp = s->pred[nth];
        s->pred[nth] = s->pred[nth]->next;
        free(temp);
        // loop to free all the node
    }
    s->pred[nth]->v = v;
    // and replace the previous vertex 
    
}


ShortestPaths dijkstra(Graph g, Vertex v){
    
    Shortest shortest = new(g,v);
    PQ pq = newPQ();
    ItemPQ first;
    first.key = v;
    first.value = 0;
    addPQ(pq,first);
    // add v to PQ
    AdjList allOut;
    shortest->dist[v] = 0;
    // set the distance of v is 0
    while(!PQEmpty(pq)){
    // if there are still some items in PQ
        ItemPQ temp = dequeuePQ(pq);
        // get the head 
        allOut = outIncident(g,temp.key);
        // loop the vertices that connect with that key
        while(allOut != NULL){
            if (shortest->dist[allOut->w] == infinity) {
                // if the vertex hasnt been visited
                ItemPQ newOne =  *newItemPQ(allOut->w, shortest->dist[temp.key] +allOut->weight);
                // create PQItem
                addPQ(pq, newOne);
                // add its distance and vertex to PQ
                addIntoPred(shortest,allOut->w,temp.key); 
                // add into pred
                shortest->dist[allOut->w] = shortest->dist[temp.key] + allOut->weight;
                // change its distance
            } else if(shortest->dist[temp.key] + allOut->weight < shortest->dist[allOut->w]){
                // if the vertex has been visited but find a shorter path
                replacePred(shortest,allOut->w,temp.key);
                // delete all the node s->pred[allOut->w] and add the new path into
                ItemPQ newOne =  *newItemPQ(allOut->w, shortest->dist[temp.key] +allOut->weight);
                addPQ(pq, newOne);
                shortest->dist[allOut->w] = shortest->dist[temp.key] + allOut->weight;                 
            } else if(shortest->dist[temp.key] + allOut-> weight == shortest->dist[allOut->w]){
                // if the vertex has beem visited but find a path whose distance is same as previous
                addIntoPred(shortest,allOut->w,temp.key);
                // add the previous to the end of s->pred[allOut->w]
                ItemPQ newOne =  *newItemPQ(allOut->w, shortest->dist[temp.key] +allOut->weight);
                addPQ(pq, newOne);
                // no need to renew the distance
            }
            allOut = allOut->next;
        }
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
    // print all the shortest path and from src and the distance
    
}


void  freeShortestPaths(ShortestPaths shortest){
    int i = 0;

    while(i < shortest.noNodes){
        PredNode* record = shortest.pred[i];
        PredNode* tempUse;
        while(record != NULL){ 
        // loop to free all the pred
            tempUse = record->next;       
            free(record);
            record = tempUse;
        }
        i++;
    }
    free(shortest.dist);
    free(shortest.pred);
    // free the array
}
// Graph ADT interface for Ass2 (COMP2521)
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include <assert.h>

//Add assert.

typedef struct GraphRep {
    int nV;
    int nE;
    AdjList *edges;
}GraphRep;

Graph newGraph(int noNodes){
    assert(noNodes > 0);
    Graph g = calloc(1,sizeof(GraphRep));
    assert(g != NULL);
    g->nV = noNodes;
    g->nE = 0;
    g->edges = malloc(g->nV * (noNodes -1) * sizeof(AdjList));
    /*for (int x = 0; x < g->nV; x++) {
        g->edges[x] = malloc(sizeof(adjListNode) * (noNodes-1));
    }  */
    return g;
}

void  insertEdge(Graph g, Vertex src, Vertex dest, int weight){
    assert(g != NULL);
    AdjList newEdge = calloc(1,sizeof(adjListNode));
    newEdge->w = dest;
    newEdge->weight = weight;
    
    AdjList temp = g->edges[src];
    if (temp == NULL) {
        g->edges[src] = newEdge;
        g->edges[src]->next = NULL;
        
    }
    if (temp->w > dest) {
       //printf("1\n");
        newEdge->next = temp;
        g->edges[src] = newEdge;
    }else{
        while(temp->next != NULL){
            if(temp->w <= newEdge->w && temp->next->w > newEdge->w){ //need to add = in < =impossible?!
                newEdge->next = temp->next;
                temp->next = newEdge;
                return;
            }else{
                temp = temp->next;
            }
        }
        if (temp->w < dest) temp->next = newEdge;
    }
}

void  removeEdge(Graph g, Vertex src, Vertex dest){
    assert(g != NULL);
    AdjList curr = g->edges[src];
    AdjList temp;
    if(curr->w == dest){
        g->edges[src] = curr->next;
        free(curr);
    }else{
        while(curr->next->w != dest){
            curr = curr->next;
        }
        temp = curr->next;
        curr->next = curr->next->next;
        free(temp);
    }
}

bool  adjacent(Graph g, Vertex src, Vertex dest){
    assert(g != NULL);
    AdjList curr = g->edges[src];
    if(curr->w == dest){
        return true;
    }else{
        while(curr->w < dest && curr->next != NULL){
            curr = curr->next;
            if(curr->w == dest){
                return true;
            }
        }
    }
    return false;
}

int  numVerticies(Graph g){
    assert(g != NULL);
    return g->nV;
}

/*
 * Returns a list of adjacent vertices
 * on outgoing edges from a given vertex.
 **/
AdjList outIncident(Graph g, Vertex v){
    assert(g != NULL);
    return g->edges[v];
}

/*
 * Returns a list of adjacent vertices
 * on incoming edges from a given vertex.
 **/
AdjList inIncident(Graph g, Vertex v){
    assert(g != NULL);
    
    int i = 0;
    AdjList temp,curr,record;
   // temp = calloc(1,sizeof(adjListNode));
    int count = 0;
    while(i < g->nV){
        if(i == v){
            continue;
        }else{
            record = g->edges[i];
            while(record != NULL){
                if(record->w == v){
                    AdjList new = calloc(1,sizeof(adjListNode));
                    new->w = i;
                    new->weight = record->weight;
                    if (curr->next == NULL) curr->next = new;
                    curr = new;
                    if (count == 0) {
                        temp = curr;
                        count = 1;
                    }
                    break;
                }
                record = record->next;
                if(record->w > v){
                    break;
                }
            }
        }
        i++;
    }
    
    return temp;
}

void  showGraph(Graph g){
    assert(g != NULL);
    printf("\n vertices=%d,edges=%d\n",g->nV,g->nE);
    int i = 0;
    int k;
    AdjList record;
    while(i <= g->nV){
        record = g->edges[i];
        k=0;
        while(record != NULL){
            while(k != record -> w){
                printf(" ");
                k=k+1;
            }
            printf("%d",record->weight);
            k=k+1;
        }
        while(k < g->nV){
            printf(" ");
            k=k+1;
        }
        printf("\n");
        i++;
    }

}


void  freeGraph(Graph g){
    assert(g != NULL);
    int i = 0;
    AdjList record,temp;
    while(i < g->nV){
        record = g->edges[i];
        while(record != NULL){
            temp = record;
            record = record->next;
            free(temp);
        }
        i++;
    }
    free(g);
}















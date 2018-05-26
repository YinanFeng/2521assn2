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
    //assert(noNodes > 0);
    Graph g = calloc(1,sizeof(GraphRep));
    //assert(g != NULL);
    g->nV = noNodes;
    g->nE = 0;
    g->edges = calloc(noNodes,sizeof(AdjList));

    return g;
}

void  insertEdge(Graph g, Vertex src, Vertex dest, int weight){
    assert(g != NULL);
    AdjList newEdge = calloc(1,sizeof(adjListNode));
    newEdge->w = dest;
    newEdge->weight = weight;
    g->nE = g->nE +1;
    
    AdjList temp = g->edges[src];
    
    if(temp == NULL){
        newEdge->next = temp;
        g->edges[src] = newEdge;
    }else{
    
        if(temp->w > dest){
            newEdge->next = temp;
            g->edges[src] = newEdge;
        }else{
            while(temp != NULL){
                if(temp->next == NULL){
                    temp->next = newEdge;
                    break;
                }else if(temp->w <= newEdge->w && temp->next->w > newEdge->w){ //need to add = in < =impossible?!
                    newEdge->next = temp->next;
                    temp->next = newEdge;
                    break;
                }else{
                    temp = temp->next;
                }
            }
        }
    }
}

void  removeEdge(Graph g, Vertex src, Vertex dest){
    assert(g != NULL);
    AdjList curr = g->edges[src];
    AdjList temp;
    g->nE = g->nE -1;
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
    temp = calloc(1,sizeof(adjListNode));
    curr = temp;
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
                    curr->next = new;
                    curr = new;
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
    AdjList incoming = temp->next;
    free(temp);
    return incoming;
}

void  showGraph(Graph g){
    assert(g != NULL);
    printf("\n vertices=%d,edges=%d\n",g->nV,g->nE);
    int i = 0;
    int k;
    AdjList record;
    while(i < g->nV){
        record = g->edges[i];
        k=0;
        while(record != NULL){
            while(k != record -> w){
                printf("x");
                k=k+1;
            }
            printf("%d",record->weight);
            k=k+1;
            record = record->next;
        }
        while(k < g->nV){
            printf("x");
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















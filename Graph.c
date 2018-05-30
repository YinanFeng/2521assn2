// Graph ADT interface for Ass2 (COMP2521)
// taken from lab08
// written by 
//     Haoyue Qiu(z5123104@unsw.edu.au) and Yinan Feng（z5167277@unsw.edu.au）

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include <assert.h>

//Add assert.

typedef struct GraphRep {
    int nV;
    // number of vertices
    int nE;
    // number of edges
    AdjList *edges;
    // array of list to store the weight of edges
}GraphRep;


Graph newGraph(int noNodes){
    Graph g = calloc(1,sizeof(GraphRep));
    g->nV = noNodes;
    g->nE = 0;
    g->edges = calloc(noNodes,sizeof(AdjList));
    // create an empty graph and allocate memory to store edges
    return g;
}


void  insertEdge(Graph g, Vertex src, Vertex dest, int weight){
    assert(g != NULL);
    AdjList newEdge = calloc(1,sizeof(adjListNode));
    newEdge->w = dest;
    newEdge->weight = weight;
    g->nE = g->nE +1;
    // create an new adjList node
    AdjList temp = g->edges[src];
    if(temp == NULL){
    // if it is first edge out from src
        newEdge->next = temp;
        g->edges[src] = newEdge;
        // just add it into graph
    }else{
        if(temp->w > dest){
            // if the first edge from src to a vertex which is bigger than dest
            newEdge->next = temp;
            g->edges[src] = newEdge;
            // treat newEdge as the first edge from src
        }else{
        // else to find the right position for the newEdge
            while(temp != NULL){
                if(temp->next == NULL){
                    // if dest is larger than all other nodes connect to src
                    temp->next = newEdge;
                    break;
                }else if(temp->w <= newEdge->w && temp->next->w > newEdge->w){
                    // find the position to add into the node
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
    // if the first edge is from src to dest
        g->edges[src] = curr->next;
        free(curr);
    }else{
        while(curr->next->w != dest){
        // keep looping until find the edge from src to dest
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
        // if there is an edge from src to dest
        return true;
    }else{
        while(curr->w < dest && curr->next != NULL){
        // keep looping until get the edge
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
    AdjList curr, temp, record;
    curr = temp = record =calloc(1, sizeof(adjListNode));
    int first = 0;
    for (int i = 0; i < g->nV; i++) {
        if (i != v) {
        // find the edge from other vertices to v
            record = g->edges[i];
            while (record != NULL) {
                //printf("%d\n", record->w);
                if (record->w > v) break;
                // if the vertex is larger than v which means there is no edge from the vertex to v
                if (record->w == v) {
                // if find the edge 
                    AdjList new = calloc(1,sizeof(adjListNode));
                    new->w = i;
                    new->weight = record->weight;
                    // add 
                    if (first == 0) {
                        temp = curr = new;
                        first = 1;
                    // the first node of incoming edge from v
                    } else {
                        curr->next = new;
                        curr = new;
                    // connect all other found edges
                    }
                    break;
                }
                record = record->next;
            }
        }
    }
    if (temp->w ==0 && temp->weight == 0) return NULL;
    // if no incoming edges
    return temp;
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
        // no edges from the vertex to itself
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
            // keep free the edges
        }
        i++;
    }
    free(g);
}


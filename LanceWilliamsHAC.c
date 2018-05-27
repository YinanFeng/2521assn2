// LanceWilliamsHAC ADT interface for Ass2 (COMP2521)
#include<stdio.h>
#include<stdlib.h>

#include "LanceWilliamsHAC.h"
#include "Dijkstra.h"
#include "PQ.h"
#define infinity 9999999


int min(int a,int b);
double updateDistance(double a,double b);



/*
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs)
 for the given graph g and the specified method for agglomerative clustering.
 Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
 For this assignment, you only need to implement the above two methods.
 
 The function returns 'Dendrogram' structure (binary tree) with the required information.
 *
 */
Dendrogram LanceWilliamsHAC(Graph g, int method){

    //Create a matrix for recording dist[a,b]
    double** distance = malloc(numVerticies(g)*sizeof(numVerticies(g)*(double*));
    int j = 0;
    while(j < numVerticies(g)){
        distance[j] = malloc(numVerticies(g)*sizeof(double));
        j++;
    }

    //Create a array of Dendrogram Node
    Dendrogram *Dgram = malloc(numVerticies(g)*sizeof(Dendrogram));
    int b = 0;
    while(b < numVerticies(g)){
        Dgram[b] = malloc(sizeof(DNode));
        //how to record vertex?
        //Is vertex matters?
        Dgram[b]->vertex = b;
        Dgram[b]->left = NULL;
        Dgram[b]->right = NULL;
        b++;
    }

    if(method == 1){
        ShortestPaths** sp = malloc(numVerticies(g)*sizeof(ShortestPaths*));
        int k = 0;
        while(k < numVerticies(g)){
            //do not need to calloc for sp[k]?
            sp[k] = dijkstra(g,k);
            k++;
        }

        //compare th distance  fill the matrix: distance.
        int z = 0;
        while(z < numVerticies(g)){
            int m = z+1;
            while(m < numVerticies(g)){
                if(sp[z]->dist[m] == 0 && sp[m]->dist[z] == 0){
                    distance[z][m] = infinity;
                }else{
                    distance[z][m] = 1/min(sp[z]->dist[m],sp[m]->dist[z])
                }
            }
        }

        int recordLast;
        int p = findShortestAndMerge(g,distance,Dgram);
        while(p != -1){
            recordLast = p;
            p = findShortestAndMerge(g,distance,Dgram);
        }
    }

    return Dgram[recordLast];
}

int min(int a,int b){
    if(a == 0){
        return b; 
    }
    if(b == 0){
        return a;
    }
    if(b <= a){
        return b;
    }
    return a;
}

void findShortestAndMerge(Graph g, double **distance, Dendrogram *Dgram){
    int shortestDistance = infinity;
    int vertexA = -1;
    int vertexB = -1;

    //find shortest distance
    int k =0;
    while(k < numVerticies(g)){
        int m = k+1;
        while(m < numVerticies(g)){
            if(distance[k][m] == 0){
                m++;
                continue;
            }
            if((shortestDistance == infinity)||(shortestDistance != infinity && distance[k][m] > shortestDistance && distance[k][m] != infinity)){
                shortestDistance = distance[k][m];
                vertexA = k;
                vertexB = m;
            }
            m++;
        }
        k++;
    }

    //merge
    if(vertexA == -1 && vertexB == -1){
        return -1;
    }else{
        Dendrogram newGram = malloc(sizeof(DNode));
        newGram->vertex = vertexA;
        newGram->left = Dgram[vertexA];
        newGram->right = Dgram[vertexB];

        //update the array
        //put the new Dgram in the smaller index vertex
        Dgram[vertexA] = newGram;
        Dgram[vertexB] = NULL;

        //update the matrix
        int q = 0;
        while(q < numVerticies(g)){
            if(q < vertexA){
                diatance[q][vertexA] = updateDistance(distance[q][vertexA],distance[q][vertexB]);
                distance[q][vertexB] = 0;
            }
            if(q == vertexA){
                distance[q][vertexB] = 0;
            }
            if(q > vertexA && q < vertexB){
                distance[vertexA][q] = updateDistance(distance[vertexA][q],distance[q][vertexB]);
                distance[q][vertexB] = 0;
            }
            if(q == vertexB){
                //do nothing
            }
            if(q > vertexB){
                distance[vertexA][q] = updateDistance(distance[vertexA][q],distance[vertexB][q]);
                distance[vertexB][q] = 0;
            }
            q++;
        }
        return vertexA;
    }
}


double updateDistance(double a,double b){
    if(a == infinity && b == infinity){
        return infinity;
    }
    if(a == infinity){
        return b;
    }
    if(b == infinity){
        return a;
    }
    if(a > b){
        return a;
    }
    return b;
}




















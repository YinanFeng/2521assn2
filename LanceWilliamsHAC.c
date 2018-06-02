// LanceWilliamsHAC ADT interface for Ass2 (COMP2521)
// written by Yinan Feng（z5167277@unsw.edu.au）and Haoyue Qiu (z5123104@unsw.edu.au)
#include<stdio.h>
#include<stdlib.h>

#include "LanceWilliamsHAC.h"


#define infinity 9999999


double min(double a,double b);
// to find the smaller number between a and b
double max(double a, double b);
// to find the larger number between a and b
int findShortestAndMerge(Graph g, double **distance, Dendrogram *Dgram, int method);
// find shortest distance and merge DNode
double updateDistance(double a,double b);
// use Complete link method to update the matrix distance after find shortest distance 
double updateLongestDistance(double a,double b);
// use Single link method to update the matrix distance after find shortest distance 
void fillDirectMatrix(Graph g,int** direct);
// find the direct edges between two vertices



/*
 The function returns 'Dendrogram' structure (binary tree) with the required information.
 *
 */
Dendrogram LanceWilliamsHAC(Graph g, int method){

    //Create a matrix for recording dist[a,b]
    double** distance = malloc(numVerticies(g)*sizeof(double*));
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
        Dgram[b]->vertex = b;
        Dgram[b]->left = NULL;
        Dgram[b]->right = NULL;
        b++;
    }

    //create a matrix to record direct distance between 2 vertex.
    //set all value in direct matrix with 0;
    int** direct = malloc(numVerticies(g)*sizeof(int*));
    int ss = 0;
    while(ss < numVerticies(g)){
        direct[ss] = malloc(numVerticies(g)*sizeof(int));
        int kk = 0;
        while (kk < numVerticies(g)){
            direct[ss][kk] = 0;
            kk++;
        }   
        ss++;
    }
    
    fillDirectMatrix(g,direct);
   
    //record the index of DNode to return
    int recordLast = 0;

        int z = 0;
        while(z < numVerticies(g)){
        // create ladder matrix to get 1/max(weight) of direct edges
            int m = z+1;
            while(m < numVerticies(g)){
                if(direct[z][m] == 0 && direct[m][z] == 0){
                // if no direct edges
                    distance[z][m] = infinity;
                }else{
                    double dA = (double)direct[m][z];
                    double dB = (double)direct[z][m];
                    distance[z][m] = 1/max(dA,dB);
                }
                m++;
            }
            z++;
        }
        
        int p = findShortestAndMerge(g,distance,Dgram,method);
        // start to find cluster and merge DNnode
        while(p != -1){
            // constantly find clusters and merge until no cluster to be merged
            recordLast = p;
            p = findShortestAndMerge(g,distance,Dgram,method);
        }

    return Dgram[recordLast];
    // return the last DNode 
}


double min(double a,double b){
    if(a == 0){
    // if no edges from m->z
        return b; 
    }
    if(b == 0){
    // if no edges from z->m
        return a;
    }
    if(b <= a){
        return b;
    }
    return a;
}

double max(double a, double b){
    if(a == 0){
        return b;
    }
    if(b == 0){
        return a;
    }
    if(a >= b){
        return a;
    }
    return b;
}



int findShortestAndMerge(Graph g, double **distance, Dendrogram *Dgram, int method){
    // initialize singleLink 
    double shortest = infinity;
    int placeA = -1;
    int placeB = -1;
    

    
    
    //find shortest distance
    int k =0;
    while(k < numVerticies(g)){
        int m = k+1;
        while(m < numVerticies(g)){
            if(distance[k][m] == 0){
                m++;
                continue;
            }
            if(distance[k][m] <= shortest){
                shortest = distance[k][m];
                placeA = k;
                placeB = m;
            }
            m++;
        }
        k++;
    }

    //merge
    if(placeA == -1 && placeB == -1){
    // all the clusters has been merged
        return -1;
    }else{
        Dendrogram newGram = malloc(sizeof(DNode));
        // create a newDNode to join cluster A and cluster B
        newGram->vertex = -1;
        newGram->left = Dgram[placeA];
        newGram->right = Dgram[placeB];

        //update the array
        //put the new Dgram in the smaller index vertex
        Dgram[placeA] = newGram;
        Dgram[placeB] = NULL;

        //update the matrix
        int q = 0;
        while(q < numVerticies(g)){
        // since the array is a ladder so there are several conditions for q

            if(q == placeA){
                // renew distance[A][B] to be set as found
                
                distance[q][placeB] = 0;
            }
            if(q < placeA){
                if(method == 2){
                    distance[q][placeA] = updateDistance(distance[q][placeA],distance[q][placeB]);
                }
                if(method == 1){
                    distance[q][placeA] = updateLongestDistance(distance[q][placeA],distance[q][placeB]);
                }
                distance[q][placeB] = 0;
                // find second cluster and merge it to the smaller vertex A
            }
            
            if(q > placeA && q < placeB){
                if(method == 2){
                    distance[placeA][q] = updateDistance(distance[placeA][q],distance[q][placeB]);
                }
                if(method == 1){
                    distance[placeA][q] = updateLongestDistance(distance[placeA][q],distance[q][placeB]);
                }
                distance[q][placeB] = 0;
            }
            if(q == placeB){
                //do nothing
            }
            if(q > placeB){
                if(method == 2){
                    distance[placeA][q] = updateDistance(distance[placeA][q],distance[placeB][q]);
                }
                if(method == 1){
                    distance[placeA][q] = updateLongestDistance(distance[placeA][q],distance[placeB][q]);
                }
                distance[placeB][q] = 0;
            }
            q++;
        }
    }
    return placeA;
}



//for completed link method
double updateDistance(double a,double b){
    // get max{cluster a, cluster b}
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

//for single link method method
double updateLongestDistance(double a,double b){
    // get min{cluster a, cluster b}
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
        return b;
    }
    return a;
}


void fillDirectMatrix(Graph g,int** direct){
    //// find the weight of direct edges
    int i = 0;
    while(i < numVerticies(g)){
        AdjList record = outIncident(g,i);
        while(record!= NULL){
            direct[i][record->w] = record->weight;
            record = record->next;
        }
        i++;
    }
}

void freeDendrogram(Dendrogram d){
    //add here...



}













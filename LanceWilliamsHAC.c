// LanceWilliamsHAC ADT interface for Ass2 (COMP2521)
#include<stdio.h>
#include<stdlib.h>

#include "LanceWilliamsHAC.h"

//#include "Dijkstra.h"
//#include "PQ.h"
#define infinity 9999999


int min(int a,int b);
int findShortestAndMerge(Graph g, double **distance, Dendrogram *Dgram);
double updateDistance(double a,double b);
//int createNewVertex(int a, int b);
void fillDirectMatrix(graph g,int** direct);


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
        //how to record vertex?
        //Is vertex matters?
        Dgram[b]->vertex = b;
        Dgram[b]->left = NULL;
        Dgram[b]->right = NULL;
        b++;
    }

    //create a matrix to record direct distance between 2 vertex.
    int** direct = malloc(numVerticies(g)*sizeof(int*));
    int ss = 0;
    while(ss < numVerticies(g)){
        direct[ss] = malloc(numVerticies(g)*sizeof(int));
    }
    //set all value in direct matrix with 0;
    int qq = 0;
    while(qq < numVerticies(g)){
        int kk = 0;
        while(kk < numVerticies(g)){
            direct[qq][kk] = 0;
            kk++;
        }
        qq++;
    }
    
    fillDirectMatrix(graph g,int** direct);
    
    
    //record the index to return
    int recordLast = 0;

    if(method == 1){

        
   //     ShortestPaths** sp = malloc(numVerticies(g)*sizeof(ShortestPaths*));
      //  int k = 0;
      //  while(k < numVerticies(g)){
            //do not need to calloc for sp[k]?
          //  sp[k] = malloc(sizeof(ShortestPaths));
         //   *sp[k] = dijkstra(g,k);
      //      k++;
        //}

        //compare th distance  fill the matrix: distance.
        int z = 0;
        while(z < numVerticies(g)){
            int m = z+1;
            while(m < numVerticies(g)){
                if(direct[z][m] == 0 && direct[m][z] == 0){
                    distance[z][m] = infinity;
                }else{
                    distance[z][m] = 1/min(direct[m][z],direct[z][m]);
                }
            }
        }


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

int findShortestAndMerge(Graph g, double **distance, Dendrogram *Dgram){
    int shortestDistance = infinity;
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
            if((shortestDistance == infinity)||(shortestDistance != infinity && distance[k][m] > shortestDistance && distance[k][m] != infinity)){
                shortestDistance = distance[k][m];
                placeA = k;
                placeB = m;
            }
            m++;
        }
        k++;
    }

    //merge
    if(placeA == -1 && placeB == -1){
        return -1;
    }else{
        Dendrogram newGram = malloc(sizeof(DNode));
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
            if(q < placeA){
                distance[q][placeA] = updateDistance(distance[q][placeA],distance[q][placeB]);
                distance[q][placeB] = 0;
            }
            if(q == placeA){
                distance[q][placeB] = 0;
            }
            if(q > placeA && q < placeB){
                distance[placeA][q] = updateDistance(distance[placeA][q],distance[q][placeB]);
                distance[q][placeB] = 0;
            }
            if(q == placeB){
                //do nothing
            }
            if(q > placeB){
                distance[placeA][q] = updateDistance(distance[placeA][q],distance[placeB][q]);
                distance[placeB][q] = 0;
            }
            q++;
        }
        return placeA;
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

void fillDirectMatrix(graph g,int** direct){
    int i = 0;
    while(i < numVerticies(g)){
        AdjList record = outIncident(g,i);
        while(record!= NULL){
            direct[i][record->w] = record->next;
            record = record->next;
        }
        i++;
    }
}







/*
//will it too big? int: overflow
int createNewVertex(int a, int b){
    int pow = 10;
    while(b >= pow){
        pow = pow * 10;
    }
    return a * pow + b;
}
*/

















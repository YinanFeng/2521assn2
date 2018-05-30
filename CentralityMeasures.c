//CentralityMeasures.c
/* Written by 
    Haoyue Qiu(z5123104@unsw.edu.au) and Yinan Feng（z5167277@unsw.edu.au）*/

#include<stdio.h>
#include<stdlib.h>
#define infinity 9999999
#include "CentralityMeasures.h"
#include "Dijkstra.h"

//int* distanceBetweenVertex(Graph g,Vertex src,ShortestPaths sp);
double numbersOfPath(Graph g,Vertex a,Vertex b,ShortestPaths sp);
// find the number of shortest paths from vertex a to vertex b

double PathThroughVertex(Graph g,Vertex a,Vertex b, Vertex v, ShortestPaths sp);
// to find the number of shortest paths from vertex a to vertex b 
//that passes through vertex v 

double distance(ShortestPaths sp, int i);
// a recucive function to find the distance for a specific node to all other nodes


void Free2dArray(int **array, int row) {  
//use to free 2d array
	for (int i = 0; i < row; i ++) {
		free(array[i]);
	}
	free(array);
}


NodeValues *newNode(Graph g) {
// create a newNode of NodeValue
	NodeValues *new = malloc(sizeof(NodeValues));
	new->noNodes = numVerticies(g);
    // noNodes = g->nV;
	new->values = calloc(new->noNodes,sizeof(double));
    //printf("%d\n",new->noNodes);
	return new;
}


NodeValues outDegreeCentrality(Graph g){
    NodeValues outNode = *newNode(g);
    // create a new struct   
    int v = 0;
    while(v < outNode.noNodes){
    // loop each vertex
        AdjList allOut = outIncident(g,v);
        // find all the vertex connects with v
        AdjList record = allOut;
        int n = 0;
        // use to count the number of vertices
        while(record != NULL){
        // if there are some vertices that can be reached from v
            n = n+1;
            record = record->next;
        }
        outNode.values[v] = n;
        v++;
    }
    return outNode;
}


NodeValues inDegreeCentrality(Graph g){
    NodeValues inNode = *newNode(g);
    // create a new struct
    int v = 0;
    while(v < inNode.noNodes){
    // loop each vertex
        AdjList allIn = inIncident(g,v);
        // find all the vertex connects with v       
        AdjList record = allIn;
        int n = 0;
        // use to count the number of vertices
        while(record != NULL){
         // if there are some vertices that can be reached from v
            n = n+1;
            record = record->next;
        }
        inNode.values[v] = n;
        v++;
    }
    return inNode;
}


NodeValues degreeCentrality(Graph g){
    NodeValues Nondirect = inDegreeCentrality(g);
    // get all the inDegreeCentrality
    NodeValues out = outDegreeCentrality(g);
    // get all the outDegreeCentrality
    for (int v = 0; v < Nondirect.noNodes; v++) {
        Nondirect.values[v] += out.values[v];
        // simplily add indegree and outdegree of each node
    }
    return Nondirect;
}


int hasPath(Graph g, Vertex v) {
// this function is used to find if there is a cycle from v
    AdjList allOut = outIncident(g,v);
    // find all the vertex connects with v
    while (allOut != NULL) {
        ShortestPaths s = dijkstra(g,allOut->w); 
        if (s.dist[v] != infinity) return 1;
        // if v can be reached from the cycle that starts from itself
        allOut = allOut->next;
    }
    return 0;
}


int reachable(Graph g, Vertex v) {
    int Path = 0;
    // to count the number of vertex that can be reached by v
    ShortestPaths sp = dijkstra(g,v);
    for(int i = 0; i < sp.noNodes; i++) {
        if (sp.dist[i] != infinity && sp.dist[i] != 0 && i != v) {
            // if there is a path from v to i
            //printf("%d-> %d  %d\n", v, i,sp.dist[i] );
            Path++;
        }
    }
    //printf("%d\n",Path);
    return (Path + hasPath(g, v)) - 1;
    //the vertex it can reach + (if it can reach itself)
}


double distance(ShortestPaths sp, int i) {
    if (i == sp.noNodes) return 0;
    // if the function called several times and is at the end of sp->dist[]
    else if (sp.dist[i]!= infinity )return sp.dist[i] + distance(sp, i+1);
    // if there is a path between them than consitantly get the distance for each node
    else return distance(sp, i+1);
    // else jump to the next node
}
NodeValues closenessCentrality(Graph g){
    NodeValues closenessNode = *newNode(g);
    // create a newNode 
    int n = 0;
    // to find closenessCentrality for each vertex
    int num = closenessNode.noNodes - 1;
    // the number of Node - 1
    while(n < closenessNode.noNodes){
        ShortestPaths sp = dijkstra(g,n);
        double dis = distance(sp, 0);
        // find the sum of distance from vertex n to all other node
        if (dis == 0) {
            n++;
            continue;
      	} else  {
            //printf("%d, %f, %d\n", n, distance, num);
            closenessNode.values[n] = reachable(g,n)/dis;
            //printf("%d\n",  reachable(g,n));
            closenessNode.values[n] = (double)reachable(g,n)/num * closenessNode.values[n];
        }    
        n++;
    } 
    return closenessNode;
}

double numbersOfPath(Graph g,Vertex a,Vertex b,ShortestPaths sp){
    if (a == b) return 0;
    PredNode* record = sp.pred[b];
    double count = 0;
    while (record != NULL) {
        if (record->v == a) {
            // if pred[b].v == a then there is a path from a to b
            count = 1;
        } else {
            // else go back to the last node
             count += numbersOfPath(g,a,record->v,sp);
        }
        record = record->next;
    }
    return count;
    
}
double PathThroughVertex(Graph g,Vertex a,Vertex b, Vertex v, ShortestPaths sp){
    PredNode* record = sp.pred[b];
    double stagePath = 0;
    while (record != NULL) {
        if (record->v == v) {
            // if its previous node is v then find how many paths go through v
            return numbersOfPath(g,a,v,sp);
        } else {
            stagePath += PathThroughVertex(g,a,record->v,v,sp);
            // else keep trace back to find if it passed through v
        }
        record = record->next;
    }
    
    return stagePath;
    
}

NodeValues betweennessCentrality(Graph g){
    NodeValues betweenessNode = *newNode(g);
    for (int x = 0; x < betweenessNode.noNodes; x++) {
        // loop each vertex
    	double count = 0;
    	for (int y = 0; y < betweenessNode.noNodes; y++) {
            // loop to find all other paths of other vertices
    		if (y != x) {
                ShortestPaths s = dijkstra(g, y);
    			for(int z = 0; z < betweenessNode.noNodes; z++) {
    				if (z != x && z != y) {    
                        // find the number of path of other vertices 
                        if (PathThroughVertex(g,y,z,x,s) != 0 && numbersOfPath(g,y,z,s) != 0)
        				count += PathThroughVertex(g,y,z,x,s)/numbersOfPath(g,y,z,s);	
                    }
    			}
            }
    	}
    	betweenessNode.values[x] = count;	
    }
    return betweenessNode;
 
    
}

NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues betweenessNode = betweennessCentrality(g);
    NodeValues Normalised = *newNode(g);
    double nV = (double)Normalised.noNodes;
    for (int i = 0; i < nV; i++) {
    // loop each node to calculate the betweenness Normalised
        Normalised.values[i] = 1/((nV-1) * (nV - 2)) * betweenessNode.values[i];
    }
    return Normalised;
}

void showNodeValues(NodeValues node){
    int i = 0;
    while(i < node.noNodes){
        printf("%d: %lf\n",i, node.values[i]);
        i++;
    }
}

void  freeNodeValues(NodeValues node){
    free(node.values);
    
    // free the array within the node
}






















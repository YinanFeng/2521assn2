//CentralityMeasures.c
#include<stdio.h>
#include<stdlib.h>
#define infinity 9999999
#include "CentralityMeasures.h"
#include "Dijkstra.h"

int* distanceBetweenVertex(Graph g,Vertex src,ShortestPaths sp);
double numbersOfPath(Graph g,Vertex a,Vertex b,ShortestPaths sp);
double PathThroughVertex(Graph g,Vertex a,Vertex b, Vertex v, ShortestPaths sp);
double distance(ShortestPaths sp, int i);
void Free2dArray(int **array, int row) {
	for (int i = 0; i < row; i ++) {
		free(array[i]);
	}
	free(array);
}
NodeValues *newNode(Graph g) {
	NodeValues *new = malloc(sizeof(NodeValues));
	new->noNodes = numVerticies(g);
	new->values = calloc(new->noNodes,sizeof(double));
    //printf("%d\n",new->noNodes);
	return new;
}
NodeValues outDegreeCentrality(Graph g){
    NodeValues outNode = *newNode(g);   
    int v = 0;
    while(v < outNode.noNodes){
        AdjList allOut = outIncident(g,v);
        AdjList record = allOut;
        int n = 0;
        while(record != NULL){
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
    int v = 0;
    while(v < inNode.noNodes){
        AdjList allIn = inIncident(g,v);       
        AdjList record = allIn;
        int n = 0;
        while(record != NULL){
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
    NodeValues out = outDegreeCentrality(g);
    for (int v = 0; v < Nondirect.noNodes; v++) {
        Nondirect.values[v] += out.values[v];
    }
    return Nondirect;
}
int hasPath(Graph g, Vertex v) {
    AdjList allOut = outIncident(g,v);
    while (allOut != NULL) {
        ShortestPaths s = dijkstra(g,allOut->w); 
        if (s.dist[v] != infinity) return 1;
        allOut = allOut->next;
    }
    return 0;
}
int reachable(Graph g, Vertex v) {
    int Path = 0;
    ShortestPaths sp = dijkstra(g,v);
    for(int i = 0; i < sp.noNodes; i++) {
        if (sp.dist[i] != infinity && sp.dist[i] != 0 && i != v) {
            //printf("%d-> %d  %d\n", v, i,sp.dist[i] );
            Path++;
        }
    }
    //printf("%d\n",Path);
    return (Path + hasPath(g, v)) - 1;
}
double distance(ShortestPaths sp, int i) {
    if (i == sp.noNodes) return 0;
    else if (sp.dist[i]!= infinity )return sp.dist[i] + distance(sp, i+1);
    else return distance(sp, i+1);
}
NodeValues closenessCentrality(Graph g){
    NodeValues closenessNode = *newNode(g);
    //printf("%d\n", closenessNode.noNodes);
    int n = 0;
    int num = closenessNode.noNodes - 1;
    while(n < closenessNode.noNodes){
        ShortestPaths sp = dijkstra(g,n);
        double dis = distance(sp, 0);
        //printf("here\n");
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
            count = 1;
        } else {
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
            return numbersOfPath(g,a,v,sp);
        } else {
            stagePath += PathThroughVertex(g,a,record->v,v,sp);
        }
        record = record->next;
    }
    
    return stagePath;
    
}

NodeValues betweennessCentrality(Graph g){
    NodeValues betweenessNode = *newNode(g);
    for (int x = 0; x < betweenessNode.noNodes; x++) {
    	double count = 0;
    	for (int y = 0; y < betweenessNode.noNodes; y++) {
    		if (y == x) {
                continue;
			}
            ShortestPaths s = dijkstra(g, y);
			for(int z = 0; z < betweenessNode.noNodes; z++) {
				if (z == x || z == y) {
                    continue;
                }
                //if (x == 55 && PathThroughVertex(g,y,z,x,s) != 0 )
                //printf("%d %d->%d PathThroughVertex %lf, numbersOfPath(g,y,z,s) %lf  %lf\n",x, y, z,PathThroughVertex(g,y,z,x,s), numbersOfPath(g,y,z,s), PathThroughVertex(g,y,z,x,s)/numbersOfPath(g,y,z,s));
                if (PathThroughVertex(g,y,z,x,s) == 0 || numbersOfPath(g,y,z,s) == 0) continue;
                
				count += PathThroughVertex(g,y,z,x,s)/numbersOfPath(g,y,z,s);	
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
}






















//CentralityMeasures.c
#include<stdio.h>
#include<stdlib.h>
#define infinity 9999999
#include "CentralityMeasures.h"
#include "Dijkstra.h"

int* distanceBetweenVertex(Graph g,Vertex src,ShortestPaths sp);
int numbersOfPath(Graph g,Vertex a,Vertex b,ShortestPaths sp);
int PathThroughVertex(Graph g,Vertex a,Vertex b, Vertex v, ShortestPaths sp);
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

//
NodeValues degreeCentrality(Graph g){
	/*NodeValues Nondirect = *newNode(g);
	int **in = malloc(Nondirect.noNodes*sizeof(int*));
	int **out = malloc(Nondirect.noNodes*sizeof(int*));
	for (int i = 0; i < Nondirect.noNodes; i++) {
		in[i] = malloc((Nondirect.noNodes -1)*sizeof(int));
		out[i] = malloc((Nondirect.noNodes -1)*sizeof(int));
		for (int j = 0; j < Nondirect.noNodes; j++) {
			if (i != j) in[i][j] = out[i][j] = 0;
		}
	}
	for (int v = 0; v < Nondirect.noNodes; v++) {
		AdjList allIn = inIncident(g,v);
		while (allIn != NULL) {
			in[v][allIn->w] = 1;
			allIn = allIn->next;
		}
	}
	for (int v = 0; v < Nondirect.noNodes; v++) {
		AdjList allOut = outIncident(g,v);
		while (allOut != NULL) {
			out[v][allOut->w] = 1;
			allOut = allOut->next;
		}
	}
	int n = 0;
    //printf("aaa\n");
	for (int i = 0; i < Nondirect.noNodes; i++) {
		for (int j = 0; j < Nondirect.noNodes; j++) {
			if (i == j) continue;
			if (out[i][j] == in[j][i] == 0) continue;
			n++;	
		}
		Nondirect.values[i] = n;
	}
	Free2dArray(in, Nondirect.noNodes);
	Free2dArray(out, Nondirect.noNodes);
    return Nondirect; */
    NodeValues Nondirect = inDegreeCentrality(g);
    NodeValues out = outDegreeCentrality(g);
    for (int v = 0; v < Nondirect.noNodes; v++) {
        Nondirect.values[v] += out.values[v];
    }
    return Nondirect;

}

NodeValues closenessCentrality(Graph g){
    NodeValues closenessNode = *newNode(g);
    int n = 0;
    while(n < closenessNode.noNodes){
        ShortestPaths sp = dijkstra(g,n);
        int total = 0, i = 0;
        while(i < closenessNode.noNodes){
            if(sp.dist[i] != infinity){
                total += sp.dist[i];
            }
            i++;
        }
        if (total == 0) closenessNode.values[n] = 0;
      	else  closenessNode.values[n] = (closenessNode.noNodes -1)/total;
        n++;
    } 
    return closenessNode;
}
int numbersOfPath(Graph g,Vertex a,Vertex b,ShortestPaths sp){
    if(a == b){
        return 0;
    }
    if(sp.pred[b]->v == a){
        return 1;
    } 
    PredNode* record = sp.pred[b];
    int stagePath = 0;
    while(record != NULL){
        stagePath = stagePath + numbersOfPath(g,a,record->v,sp);
        record = record->next;
    }
    
    return stagePath;
    
}
int PathThroughVertex(Graph g,Vertex a,Vertex b, Vertex v, ShortestPaths sp){
    if(a == b){
        return 0;
    }
    if(sp.pred[b]->v == v){
        return 1;
    } 
    PredNode* record = sp.pred[b];
    int stagePath = 0;
    while(record != NULL){
        stagePath = stagePath + numbersOfPath(g,a,record->v,sp);
    }
    
    return stagePath;
    
}

NodeValues betweennessCentrality(Graph g){
    NodeValues betweenessNode = *newNode(g);
    for (int x = 0; x < betweenessNode.noNodes; x++) {
    	int count = 0;
    	for (int y = 0; y < betweenessNode.noNodes; y++) {
    		if (y == x) continue;
			ShortestPaths s = dijkstra(g, y);
			for(int z = 0; z < betweenessNode.noNodes; z++) {
				if (z == x || z == y) continue;
				if (PathThroughVertex(g,y,z,x,s) == 0 || numbersOfPath(g,y,z,s) == 0) {
					count = 0;
					continue;	
				} 	
				count += PathThroughVertex(g,y,z,x,s)/numbersOfPath(g,y,z,s);	
			}
    	}
    	betweenessNode.values[x] = count;	
    }
    return betweenessNode;
 
    
}


int* distanceBetweenVertex(Graph g,Vertex src,ShortestPaths sp){
    int* distance = calloc(numVerticies(g),sizeof(int));

    int i = 0;
    while(i < numVerticies(g)){
        if(sp.dist[i] == infinity){
            distance[i] = infinity;
        }else{
            int total = 0;
            PredNode* record = sp.pred[i];
            total = total + sp.dist[i];
            while(record != NULL){
                total = total + sp.dist[record->v];
                record = sp.pred[record->v];
            }
            distance[i] = total;
        }
        i++;
    }
    
    return distance;
}



NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues betweeness = betweennessCentrality(g);
    NodeValues norm = *newNode(g);
    int max = 0;
    int min = infinity;
    int n = 0;
    while(n < norm.noNodes){
        if(betweeness.values[n] > max){
            max = betweeness.values[n];
        }
        if(betweeness.values[n] < min){
            min = betweeness.values[n];
        }
        n++;
    }
    int k = 0;
    while(k < norm.noNodes){
        norm.values[k] = (betweeness.values[k] - min)/(max-min);
        k++;
    }
    freeNodeValues(betweeness);
    return norm;
}

void showNodeValues(NodeValues node){
    printf("NumberOfNodes: %d\n",node.noNodes);
    int i = 0;
    while(i < node.noNodes){
        printf("%f  \n",node.values[i]);
        i++;
    }
}

void  freeNodeValues(NodeValues node){
    free(node.values);
}






















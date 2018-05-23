//CentralityMeasures.c
#include<stdio.h>
#include<stdlib.h>

#define infinity 9999999
#include "CentralityMeasures.h"
#include "Dijkstra.h"


int* distanceBetweenVertex(Graph g,Vertex src,ShortestPaths sp);
int numbersOfPath(Graph g,Vertex a,Vertex b,ShortestPaths sp);


NodeValues outDegreeCentrality(Graph g){
    NodeValues outNode;
    outNode.noNodes = numVerticies(g);
    outNode.values = calloc(outNode.noNodes,sizeof(double));
    int v = 0;
    
    while(v < outNode.noNodes){
        AdjList allOut = inIncident(g,v);
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
    NodeValues inNode;
    inNode.noNodes = numVerticies(g);
    inNode.values = calloc(inNode.noNodes,sizeof(double));
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



//?????????????return inDegreeCentrality/outDegreeCentrality
NodeValues degreeCentrality(Graph g){
    return outDegreeCentrality(g);
}

//Why 分母(n-1)nodes 不是n吗 所有reachable的都要加上去？？？(n-1)nodes
NodeValues closenessCentrality(Graph g){
    NodeValues closenessNode;
    closenessNode.noNodes = numVerticies(g);
    closenessNode.values = calloc(closenessNode.noNodes,sizeof(double));

    int n = 0;
    while(n < closenessNode.noNodes){
        ShortestPaths sp = dijkstra(g,n);
        int total = 0, i = 0, counter = 0;
        while(i < closenessNode.noNodes){
            if(sp.dist[i] != infinity){
                counter = counter + 1;
                int k=0;
                PredNode* record = sp.pred[i];
                k = i;
                while(record != NULL){
                    total = total + sp.dist[k];
                    k = record->v;
                    record = sp.pred[record->v];
                }
            }
            i++;
        }
        closenessNode.values[n] = counter/(closenessNode.noNodes-1)*counter/total;
        n++;
    }
    
    return closenessNode;
}

NodeValues betweennessCentrality(Graph g){
    NodeValues betweenessNode;
    betweenessNode.noNodes = numVerticies(g);
    betweenessNode.values = calloc(betweenessNode.noNodes,sizeof(double));
    
    ShortestPaths* sp = calloc(betweenessNode.noNodes,sizeof(ShortestPaths));
    int k = 0;
    while(k<betweenessNode.noNodes){
        sp[k] = dijkstra(g,k);
        k++;
    }
    
    int** allDistance = calloc(betweenessNode.noNodes,sizeof(int*));
    int p = 0;
    while(p<betweenessNode.noNodes){
        allDistance[p] = distanceBetweenVertex(g,p,sp[p]);
        p++;
    }
    
    //Some num of path do not need to count  没有经过v, 所以对 numofpath 不用全部算出来 可能浪费时间 需要的时候算即可


    int z = 0;
    while(z < betweenessNode.noNodes){
        double total = 0;
        int n = 0;
        while(n < betweenessNode.noNodes){
            if(n == z){
            }else{
                int m = 0;
                while(m < betweenessNode.noNodes){
                    if(m == z){
                    }else{
                        if(allDistance[n][z] + allDistance[z][m] != allDistance[n][m]){
                        }else{
                            total = total + numbersOfPath(g,n,z,sp[n])*numbersOfPath(g,z,m,sp[z])/numbersOfPath(g,n,m,sp[n]);
                        }
                    }
                }
            }
            n++;
        }
        betweenessNode.values[z] = total;
        z++;
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

NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues betweeness = betweennessCentrality(g);
    NodeValues norm;
    norm.noNodes = numVerticies(g);
    norm.values = calloc(betweeness.noNodes,sizeof(double));
    
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
    
    return norm;
 
}

void showNodeValues(NodeValues node){
    printf("NumberOfNodes: %d\n",node.noNodes);
    int i = 0;
    while(i < node.noNodes){
        printf("%f  ",node.values[i]);
        i++;
    }
}

void  freeNodeValues(NodeValues node){
    //needed???
    free(node.values);
   //????????? free(node);
}






















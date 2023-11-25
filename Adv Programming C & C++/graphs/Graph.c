//-----------------------------------------------------------------------------------------
// Author: 
// 	Adrian Murillo
// CruzID: 
// 	amurill9
// ID#: 
// 	1573260
// Institution:
// 	UC Santa Cruz
// Course:
// 	CMPS 101
// File Name:
// 	Graph.c
// File Description:
// 	Function implementations for Graph ADT
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include "Graph.h"

/*** Constructors-Destructors ***/
Graph newGraph(int n){
	if(n>0){
		Graph g = malloc(sizeof(Graph)+n*n*sizeof(List)+MAX_SIZE*5*sizeof(int));
		g->vertices = n;
		g->edges = 0;
		g->source = 0;
		int i;
		for(i=0;i<n;i++){
			g->graph[i] = NULL;
		}
		return g;
	} else{
		fprintf(stderr, "Incorrect input!\n");
		return NULL;
	}
}
void freeGraph(Graph* pG){
	int i;
	Graph G = *pG;
	for(i=0;i<=getOrder(G);i++){
		freeList(&G->graph[i]);
	}
	free(*pG);
	pG = NULL;
}
/*** Access functions ***/
int getOrder(Graph G){
	return G->vertices;
}
int getSize(Graph G){
	return G->edges;
}
int getSource(Graph G){
	return G->source;
}
int getParent(Graph G, int u){
	return G->parent[u];
}
int getDist(Graph G, int u){
	if(G->source==0){
		return INF;
	}
	return G->distance[u];
}
void getPath(List path, Graph G, int b){
    int parent = G->parent[b];
    if(G->distance[b]==INF){
    	append(path,NIL);
    	return;
    }
    append(path,b);
    while(parent!=NIL){
       prepend(path,parent);
       parent = G->parent[parent];
    }
}
/*** Manipulation procedures ***/
void makeNull(Graph G){
	int i;
	for(i=0;i<=getOrder(G);i++){
		G->graph[i] = NULL;
	}
}
void changeEntry(Graph G, int a, int b){
	List list = G->graph[a];
	//printf("Inserting new entry: (%i,%i)\n",a,b);
	if(list!=NULL){
		//printf("List not empty\n");
		if(b==1){
			prepend(list,b);
			return;
		} else{
			int column;
			for(moveFront(list);index(list)!=-1;moveNext(list)){
				column = get(list);
				if(column==b){
					return;
				}
				if(column>b){
					insertBefore(list,b);
					return;
				}
			}
			append(list,b);
			return;
		}
	} else{
		//printf("List empty\n");
		G->graph[a] = newList();
		append(G->graph[a],b);
	}
}
void addEdge(Graph G, int u, int v){
	if(u>0&&v>0&&G!=NULL&&u<=getOrder(G)&&v<=getOrder(G)){
		G->edges++;
		changeEntry(G,u,v);
		changeEntry(G,v,u);
	}
}
void addArc(Graph G, int u, int v){
	if(u>0&&v>0&&G!=NULL&&u<=getOrder(G)&&v<=getOrder(G)){
		G->edges++;
		changeEntry(G,u,v);
	}
}
void BFS(Graph G, int s){
	G->source=s;
	int i;
	for(i=1;i<=getOrder(G);i++){
		if(i!=s){
			G->color[i] = white;
			G->distance[i] = INF;
			G->parent[i] = NIL;
		}
	}
	G->color[s] = grey;
	G->distance[s] = 0;
	G->parent[s] = NIL;
	int x;
	List Q = newList();
	prepend(Q,s);
	while(front(Q)!=-1){
		x = back(Q);
		deleteBack(Q);
		List l = G->graph[x];
		if(l!=NULL){
			for(moveFront(l);index(l)!=-1;moveNext(l)){
				int y = get(l);
				if(G->color[y]==white){
					G->color[y]=grey;
					G->distance[y]=G->distance[x]+1;
					G->parent[y]=x;
					prepend(Q,y);
				}
			}
		}
		G->color[x]=black;
	}
	freeList(&Q);
}
/*** Other operations ***/
void printGraph(FILE* out, Graph G){
	int i;
	List l;
	for(i=1;i<=getOrder(G);i++){
		fprintf(out,"%i: ",i);
		l = G->graph[i];
		if(l!=NULL){
			for(moveFront(l);index(l)!=-1;moveNext(l)){
				fprintf(out, " %i",get(l));
			}
		}
		fprintf(out,"\n");
	}
}
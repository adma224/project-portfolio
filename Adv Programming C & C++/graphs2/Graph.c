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
		int i;
		for(i=0;i<n;i++){
			g->graph[i] = NULL;
			g->f[i]=UNDEF;
			g->d[i]=UNDEF;
			g->parent[i]=NIL;
			g->color[i]=UNDEF;
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
int getParent(Graph G, int u){
	return G->parent[u];
}
int getDiscover(Graph G, int u){
	if(u>=1&&u<=getOrder(G)){
		return G->d[u];
	}
	return 0;
}
int getFinish(Graph G, int u){
	if(u>=1&&u<=getOrder(G)){
		return G->f[u];
	}
	return 0;
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
void visit(Graph G, List S, int x, int* time){
	G->d[x] = ++*time;
	G->color[x] = grey;
	List l = G->graph[x];
	if(l!=NULL){
		for(moveFront(l);index(l)!=-1;moveNext(l)){
			int y=get(l);
			if(G->color[y]==white){
				G->parent[y]=x;
				visit(G,S,y,time);
			}
		}
	}
	G->color[x]=black;
	G->f[x]=++*time;
	prepend(S,x);
}
void DFS(Graph G, List S){ /* Pre: length(S)==getOrder(G) */
	if(length(S)==getOrder(G)){
		int i;
		int time = 0;
		int order[getOrder(G)];
		i=0;
		for(moveFront(S);index(S)!=-1;moveNext(S)){
			order[i]=get(S);
			i++;
		}
		for(i=0;i<getOrder(G);i++){
			G->color[order[i]]=white;
   			G->parent[order[i]]=NIL;
		}
		clear(S);
		for(i=0;i<getOrder(G);i++){
			int x = order[i];
			if(x!=0&&G->color[x]==white){
				visit(G,S,x,&time);
			} else if(x==0){
				return;			}
		}
	}
}
/*** Other operations ***/
Graph transpose(Graph G){
	Graph R = newGraph(getOrder(G));
	int i;
	for(i=1;i<=getOrder(G);i++){
		List list = G->graph[i];
		if(list!=NULL){
			for(moveFront(list);index(list)!=-1;moveNext(list)){
				changeEntry(R,get(list),i);
			}
		}
		R->edges=G->edges;
	}
	return R;
}
Graph copyGraph(Graph G){
	Graph R = newGraph(getOrder(G));
	int i;
	for(i=1;i<=getOrder(G);i++){
		List list = G->graph[i];
		if(list!=NULL){
			R->graph[i]=copyList(list);
		}
		R->d[i]=G->d[i];
		R->f[i]=G->f[i];
		R->color[i]=G->color[i];
		R->parent[i]=G->parent[i];
	}
	R->edges=G->edges;
	R->vertices=G->vertices;
	return R;
}
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
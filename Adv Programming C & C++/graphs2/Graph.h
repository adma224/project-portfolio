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
// 	Graph.h
// File Description:
// 	Header file for Graph ADT
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

#define MAX_SIZE 256


#define white 0
#define grey 1
#define black 2
#define UNDEF 0
#define NIL 0
struct G{
	int vertices;
	int edges;
	List graph[MAX_SIZE];
	int d[MAX_SIZE];
	int f[MAX_SIZE];
	int color[MAX_SIZE];
	int parent[MAX_SIZE];
} typedef GraphObj;

typedef GraphObj* Graph;

/* Constructors-Destructors */
Graph newGraph(int n);
void freeGraph(Graph* pG);
/* Access functions */
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
/* Manipulation procedures */
void addArc(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void DFS(Graph G, List S); /* Pre: length(S)==getOrder(G) */
/* Other Functions */
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out , Graph G);
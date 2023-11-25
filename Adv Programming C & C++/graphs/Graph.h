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
#define INF -1
#define NIL 0
struct G{
	int vertices;
	int edges;
	int source;
	List graph[MAX_SIZE];
	int distance[MAX_SIZE];
	int color[MAX_SIZE];
	int parent[MAX_SIZE];
} typedef GraphObj;

typedef GraphObj* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);
/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u);
int getDist(Graph G, int u);
void getPath(List L, Graph G, int u);
/*** Manipulation procedures ***/
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);
/*** Other operations ***/
void printGraph(FILE* out, Graph G);
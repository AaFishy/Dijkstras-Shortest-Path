// Written by Aaron Lin
// Includes functions assisting with the running of dijkstra
// and the dijkstra function which returns shortestPath from a given vertex
//
// Uses Dijkstra shortest path method to detemrine shortest path to a given vertex from source

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

// Initialises a new item of struct ItemPQ from PQ.h (PQ = priority queue)
static ItemPQ *newItem (int key, int value) {
    ItemPQ *item = malloc(sizeof(ItemPQ));
    item->key = key;
    item->value = value;
    
    return item;
}

// Function that returns a ShortestPath struct containing the shortest path from src
ShortestPaths dijkstra(Graph g, Vertex src) {
    // makes sure the graph given exists
    assert(g != NULL);
    ShortestPaths sps;
    // #Nodes in ShortestPaths struct == #Vertices in graph
    sps.numNodes = GraphNumVertices(g);
    // Initialises seen list to determine which vertex we already visited
    Vertex seen[sps.numNodes];
    // Initialises dist and pred list
    sps.dist = malloc(sps.numNodes * sizeof(INT_MAX));
    sps.pred = malloc(sps.numNodes * sizeof(PredNode));
    sps.pred[src] = NULL;
    // Sets all seen to 0 (not seen) dist to INT_MAX (max dist) pred array to NULL (no preds)
    for (int i = 0; i < sps.numNodes; i++) {
        seen[i] = 0;
        sps.dist[i] = INT_MAX;
        sps.pred[i] = NULL;
    }
    // Source has now been seen
    seen[src] = 1;
    // Distance to src is 0 as it is distance from itself
    sps.dist[src] = 0;
    // source for this shortestpath struct is src
    sps.src = src;
    
    // new PriorityQ for the path
    PQ pathQ = PQNew();
    // Creates the first item as source with value 0
    ItemPQ *firstItem = newItem(src, 0);
    // Adds the first item to pathQ
    PQAdd(pathQ, *firstItem);
    // Free values in firstItem
    free(firstItem);
    
    // Loops while there are still items in pathQ
    while (!PQIsEmpty(pathQ)) {
        // Dequeue from path queue
        ItemPQ source = PQDequeue(pathQ);
        // Loops through the graph via the dequeued item
        for (AdjList n = GraphOutIncident(g, source.key); n != NULL; n = n->next) {
            // n is not seen
            if (!seen[n->v]) {
                // Create temp item called new
                ItemPQ *new = newItem(n->v, n->weight);
                // Adds new to pathQ
                PQAdd(pathQ, *new);
                // Frees the temp variable
                free(new);
                // Vertice of n is now seen
                seen[n->v] = 1;
            }
            // weight is the weight of n + weight of source (weight == value at the vertex)
            int weight = n->weight + source.value;
            // Initialise a temp variable named 'temp' to be NULL wont be 
            PredNode *temp = NULL;
            // Weight is less than dist from source to n->v (dist == distance of connecting edge)
            if (weight < sps.dist[n->v]) {
                // initialise temp
                temp = malloc(sizeof(PredNode));
                temp->v = source.key;
                temp->next = NULL;
                sps.pred[n->v] = temp;
                sps.dist[n->v] = weight;
                // Creating new ItemPQ to update weight
                ItemPQ update;
                update.key = n->v;
                update.value = weight;
                PQUpdate(pathQ, update);
            // Weight equal to dist from source to n->v
            } else if (weight == sps.dist[n->v]) {
                temp = malloc(sizeof(PredNode));
                temp->v = source.key;
                temp->next = sps.pred[n->v];
                sps.pred[n->v] = temp;
            }
        }
    }
    // if the dist is still INT_MAX, it is unreachable and hence dist = 0 for those
    for (int i = 0; i < sps.numNodes; i++) {
        if (sps.dist[i] == INT_MAX) sps.dist[i] = 0;
    }
    
    PQFree(pathQ);
    return sps;
}

void showShortestPaths(ShortestPaths sps) {
    printf("The shortest path from %d \n", sps.src);
	for (int i = 0; i < sps.numNodes; i++) {
		printf("to Node %d: Distance is %d\n", i, sps.dist[i]);
		printf("Prev Nodes: ");
		for (PredNode *n = sps.pred[i]; n != NULL; n = n->next) {
			printf("%d ->", n->v);
		}
		printf("X\n");
	}
}

// Free everything inside the shortestPath struct
void freeShortestPaths(ShortestPaths sps) {
    free(sps.dist);
    PredNode *currentFree = NULL;
	for (int i = 0; i < sps.numNodes; i ++) {
	    for (PredNode *n = sps.pred[i]; n != NULL; n = n->next) {
		    if (currentFree != NULL) free(currentFree);
		    currentFree = n;
	    }
	}
	if (currentFree != NULL) free(currentFree);
	free(sps.pred);
}
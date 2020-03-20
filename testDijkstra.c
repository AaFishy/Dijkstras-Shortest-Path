// Written by Aaron Lin
// Function to test Dijkstra.c with a main function
#include "Dijkstra.h"
#include "GraphRead.h"

#include <stdio.h>
#include <stdlib.h>

// Function to display the ShortestPath struct in Dijkstra.h
void displayShortestPathsStruct(ShortestPaths sps) {
	int i = 0;
	printf("Node %d\n", sps.src);
	printf("  Distance\n");
	// Prints the distances from each node
	for (i = 0; i < sps.numNodes; i++) {
		if (i == sps.src)
			printf("    %d : X\n", i);
		else
			printf("    %d : %d\n", i, sps.dist[i]);
	}
	printf("  Preds\n");
	// Prints the predecessors
	for (i = 0; i < sps.numNodes; i++) {
		int numPreds = 0;
		int preds[sps.numNodes];
		printf("    %d : ",i);
		PredNode *curr = sps.pred[i];
		while (curr != NULL && numPreds < sps.numNodes) {
			preds[numPreds++] = curr->v;
			curr = curr->next;
		}
		
		// Insertion sort
		for (int j = 1; j < numPreds; j++) {
			int temp = preds[j];
			int k = j;
			while (k > 0 && preds[k - 1] > temp) {
				preds[k] = preds[k - 1];
				k--;
			}
			preds[k] = temp;
		}
		
		for (int j = 0; j < numPreds; j++) {
			printf("[%d]->", preds[j]);
		}
		printf("NULL\n");
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: ./testDijkstra [file]\n");
		return EXIT_FAILURE;
	}
	
	Graph g = readGraph(argv[1]);
	int i;
	for (i = 0; i < GraphNumVertices(g); i++) {
	    // Calls dijkstra function from dijkstra.h which returns the shortest path
		ShortestPaths paths = dijkstra(g, i);
		// Displays the path
		displayShortestPathsStruct(paths);
		// Frees the memory in paths
		freeShortestPaths(paths);
	}
	// Frees the graph
	GraphFree(g);
}

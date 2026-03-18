/*
	Implementation of a vertex for a graph.

	Author: Luca Ardanaz
*/

#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "Edge.h"

using namespace std;

class Vertex {
private:
	char label;
	vector<Edge*> edges;

public:
	Vertex(char nLabel) {
		label = nLabel;
	}

	~Vertex() {}

	void addEdge(double nValue, Vertex* nVertex) {
		edges.push_back(new Edge(nValue, nVertex));
	}

	char getLabel() {
		return label;
	}
};

#endif

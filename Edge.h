/*
	Implementation of an edge for a graph.

	Author: Luca Ardanaz
*/

#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"

class Vertex;

class Edge {
private:
	double value;
	Vertex* vertex;

public:
	Edge(double nValue, Vertex* nVertex) {
		value = nValue;
		vertex = nVertex;
	}

	~Edge() {}

	double getValue() {
		return value;
	}

	Vertex* getVertex() {
		return vertex;
	}
};

#endif

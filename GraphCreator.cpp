/*
   A text-based graph creator with an adjacency table visualization and Dijkstra's algorithm.

Author: Luca Ardanaz
 */

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Vertex.h"

using namespace std;

queue<string> tokenize(string str);
Vertex* getVertexByLabel(char label, vector<Vertex*> vertices);

int main() {

	string input = "";
	queue<string> tokens;
	vector<Vertex*> vertices;

	do {

		// --<< GET USER INPUT >>--
		cout << endl << endl;
		cout << "Enter command (enter 'help' for commands):" << endl;
		getline(cin, input);
		tokens = tokenize(input);		

		if (tokens.empty()) {
			continue;
		}

		// --<< RUN CORRESPONDING COMMAND >>--

		if (tokens.front() == "help") {
			cout << endl;
			cout << "Commands:" << endl;
			cout << "av [label] - add vertex with unique label" << endl;
			cout << "ae [vertex 1] [vertex 2] [value] - add edge between vertecies with value" << endl;
			cout << "rv [label] - remove vertex by label" << endl;
			cout << "re [vertex 1] [vertex 2] - remove edge between vertecies" << endl;
			cout << "p - print adjacency table" << endl;
			cout << "f - find shortext path" << endl;
			cout << "q - quit" << endl;
			continue;
		}			

		if (tokens.front() == "av") {
			tokens.pop();

			//isolate label

			char label = ' ';
			try {
				label = tokens.front()[0];
			}
			catch(...) {
				cout << "Invalid token for command 'av'" << endl;
				continue;
			}

			//confirm that the label is unique
			if (getVertexByLabel(label, vertices) != nullptr) {
				cout << "That label is already taken" << endl;
				continue;
			}

			//add the vertex
			vertices.push_back(new Vertex(label));

			continue;
		}

		if (tokens.front() == "ae") {
			tokens.pop();

			//get labels and value
			char startV = ' ';
			char endV = ' ';
			double val = 0.0;

			try {			
				startV = tokens.front()[0];
				tokens.pop();
				endV = tokens.front()[0];
				tokens.pop();
				val = stod(!tokens.empty() ? tokens.front() : "a"); 
			}
			catch(...) {
				cout << "Invalid tokens for command 'ae'" << endl;
				continue;
			}

			//get the vertices
			Vertex* start = getVertexByLabel(startV, vertices);	
			Vertex* end = getVertexByLabel(endV, vertices);	
		
			//ensure vertex validity
			if (start == nullptr || end == nullptr || start == end) {
				cout << "Invalid vertices" << endl;
				continue;
			}

			//create the edge
			start -> addEdge(val, end);
	
		}	

		if (tokens.front() == "p") {

			cout << endl;

			if (!vertices.empty()) {

				//create empty table
				int dim = vertices.size();
				vector<vector<bool>> table(dim, vector<bool>(dim, false));

				//determine values
				
	
				//print table
				cout << "Dim: " << dim << endl;
				
				string header = "  ";
				for (Vertex* vertex : vertices) {
					header += vertex -> getLabel();
					header += " ";
				}

				cout << header << endl;

				for (int i = 0; i < table.size(); i++) {
					
					string rowStr = "";
					rowStr += vertices.at(i) -> getLabel();
					rowStr += " ";

					for (bool val : table.at(i)) {
						rowStr += (val ? "T" : "F");
						rowStr += " ";
					}

					cout << rowStr << endl;
				}
			}

			else {
				cout << "Add vertices first" << endl;
			}

			continue;
		}	

	} while (tokens.front() != "q");	

	return 0;
}

queue<string> tokenize(string str) {
	queue<string> tokens;

	string token = "";
	for (char c : str) {
		if (c == ' ') {
			tokens.push(token);
			token = "";
		}
		else {
			token += c;
		}
	}

	tokens.push(token);

	return tokens;
}

Vertex* getVertexByLabel(char label, vector<Vertex*> vertices) {
	for (Vertex* v : vertices) {
		if (v -> getLabel() == label) {
			return v;
		}	
	}
	
	return nullptr;
}

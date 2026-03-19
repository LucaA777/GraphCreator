/*
   A text-based graph creator with an adjacency table visualization and Dijkstra's algorithm.

Author: Luca Ardanaz
 */

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include "Vertex.h"

using namespace std;

queue<string> tokenize(string str);

struct edge {
	char start;
	char end;
	double value;
};

//this is necessary since I want a set of structs for edges
//source: https://stackoverflow.com/questions/5816658/how-to-have-a-set-of-structs-in-c
bool operator<(const edge& e1, const edge& e2) {
	return e1.start < e2.start;
}

int main() {

	string input = "";
	queue<string> tokens;
	set<char> vertices;
	set<edge> edges;

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
				if (tokens.size() != 1) {
					//throw error if token count isn't 1
					throw 1000;
				}

				label = tokens.front()[0];
			}
			catch(...) {
				cout << "Invalid token for command 'av'" << endl;
				continue;
			}

			//confirm that the label is unique
			if (vertices.count(label)) {
				cout << "Label taken" << endl;
				continue;
			}

			//add the vertex
			vertices.insert(label);

			continue;
		}

		if (tokens.front() == "ae") {
			tokens.pop();

			//get labels and value
			char start = ' ';
			char end = ' ';
			double val = 0.0;

			try {			
				if (tokens.size() != 3) {
					//throws a random number if the size doesn't match
					throw 1000;
				}

				start = tokens.front()[0];
				tokens.pop();
				end = tokens.front()[0];
				tokens.pop();
				val = stod(tokens.front()); 
			}
			catch(...) {
				cout << "Invalid tokens for command 'ae'" << endl;
				continue;
			}

			//ensure vertex validity
			if (start == ' ' || end == ' ' || start == end) {
				cout << "Invalid vertices" << endl;
				continue;
			}

			//check that an edge doesn't already exist between these two vertices
			bool duplicate = false;
			for (edge e : edges) {
				if (e.start == start && e.end == end) {
					cout << "Duplicate edge, aborted" << endl;
					duplicate = true;
					break;
				}
			}

			if (duplicate) {
				continue;
			}

			//add the edge
			edges.insert({start, end, val});
	
		}	

		if (tokens.front() == "rv") {
			tokens.pop();

			//isolate label

			char label = ' ';
			try {
				if (tokens.size() != 1) {
					//throw error if token count isn't 1
					throw 1000;
				}

				label = tokens.front()[0];
			}
			catch(...) {
				cout << "Invalid token for command 'rv'" << endl;
				continue;
			}

			//confirm that the label is in the set
			if (!vertices.count(label)) {
				cout << "No match" << endl;
				continue;
			}

			//remove edges
			//source: https://stackoverflow.com/questions/2874441/deleting-elements-from-stdset-while-iterating
			for (auto it = edges.begin(); it != edges.end(); ) {
				if (it -> start == label || it -> end == label) {
					edges.erase(it++);
				}
				else {
					++it;
				}
			}

			//remove the vertex
			vertices.erase(label);			

			continue;
		}

		if (tokens.front() == "re") {
			tokens.pop();

			//get labels
			char start = ' ';
			char end = ' ';

			try {			
				if (tokens.size() != 2) {
					//throws a random number if the size doesn't match
					throw 1000;
				}

				start = tokens.front()[0];
				tokens.pop();
				end = tokens.front()[0];
			}
			catch(...) {
				cout << "Invalid tokens for command 're'" << endl;
				continue;
			}

			//ensure vertex validity
			if (start == ' ' || end == ' ' || start == end) {
				cout << "Invalid vertices" << endl;
				continue;
			}

			//check that an edge exists between these two vertices
			edge match;
			bool exists = false;
			for (edge e : edges) {
				if (e.start == start && e.end == end) {
					match = e;
					exists = true;
					break;
				}	
			}

			if (!exists) {
				cout << "No matching edge" << endl;
				continue;
			}

			//remove the edge
			edges.erase(match);
	
		}	

		if (tokens.front() == "p") {

			cout << endl;

			if (!vertices.empty()) {

				//create empty table
				int dim = vertices.size();
				vector<vector<bool>> table(dim, vector<bool>(dim, false));
	
				//print table
				cout << "Dim: " << dim << endl;
				
				//determine connections
				for (edge e : edges) {
					int startIndex = distance(vertices.begin(), vertices.find(e.start));
					int endIndex = distance(vertices.begin(), vertices.find(e.end));
					table[endIndex][startIndex] = true;
				}


				//concatenate table header line
				string header = "  ";
				for (char vertex : vertices) {
					header += vertex;
					header += " ";
				}

				cout << header << endl;

				for (int i = 0; i < table.size(); i++) {
					
					string rowStr = "";
					rowStr += header[2 * i + 2];
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

int getVertexIndex(Vertex* vertex, vector<Vertex*> vertices) {
	//find match, return the index
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices.at(i) == vertex) {
			return i;
		}
	}

	//returns -1 if no match was found
	return -1;
}

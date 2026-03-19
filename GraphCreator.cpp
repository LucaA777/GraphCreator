/*
   A text-based graph creator with an adjacency table visualization and Dijkstra's algorithm.

Author: Luca Ardanaz
 */

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <limits>
#include <stack>

using namespace std;

const bool DEBUG = false;

queue<string> tokenize(string str);

struct edge {
	char start;
	char end;
	double value;
};

void dijkstraPathfinding(char start, char target, set<char> vertices, set<edge> edges);

//this is necessary since I want a set of structs for edges
//source: https://stackoverflow.com/questions/5816658/how-to-have-a-set-of-structs-in-c
//set duplicate control is done by checking if neither e1 < e2 nor e2 < e1
bool operator<(const edge& e1, const edge& e2) {
	return e1.start < e2.start || e1.end < e2.end;
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
			cout << "\tadding -d adds edge in both directions" << endl;
			cout << "rv [label] - remove vertex by label" << endl;
			cout << "re [vertex 1] [vertex 2] - remove edge between vertecies" << endl;
			cout << "p - print adjacency table" << endl;
			cout << "f [start] [end] - find shortext path" << endl;
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
			bool bidirectional = false;

			try {			
				if (tokens.size() != 3 && tokens.size() != 4) {
					//throws a random number if the size doesn't match
					throw 1000;
				}

				start = tokens.front()[0];
				tokens.pop();
				end = tokens.front()[0];
				tokens.pop();
				val = stod(tokens.front());
			       	tokens.pop();

				if (!tokens.empty()) {
					bidirectional = tokens.front() == "-d";

					//if not bidirectional, but there are 4 tokens, error
					if (!bidirectional) {
						throw 2000;
					}	
				}
					
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

				if (bidirectional && e.start == end && e.end == start) {
					cout << "Duplicate edge, aborted" << endl;
					duplicate = true;
					break;
				}
			}

			if (duplicate) {
				continue;
			}

			//add the edge
			if (DEBUG) {
				cout << "Inserting edge from " << start << " to " << end << " with value: " << val << endl;
			}
			edges.insert({start, end, val});

			//insert in opposite direction aswell if bidirectional
			if (bidirectional) {
				if (DEBUG) {
					cout << "Inserting edge from " << end << " to " << start << " with value: " << val << endl;
				}

				edges.insert({end, start, val});
			}

			continue;

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
			edge reverseMatch;
			bool exists = false;
			bool reverseExists = false;
			for (edge e : edges) {
				if (e.start == start && e.end == end) {
					match = e;
					exists = true;
				}	
				if (e.start == end && e.end == start) {
					reverseMatch = e;
					reverseExists = true;
				}
			}

			if (!exists) {
				cout << "No matching edge" << endl;
				continue;
			}

			//remove the edges
			else {
				edges.erase(match);
			}

			if (reverseExists) {
				edges.erase(reverseMatch);
			}

			continue;

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

		if (tokens.front() == "f") {
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
				cout << "Invalid tokens for command 'f'" << endl;
				continue;
			}

			//ensure vertex validity
			if (start == ' ' || end == ' ' || start == end) {
				cout << "Invalid vertices" << endl;
				continue;
			}

			//run pathfinding
			if (DEBUG) {
				cout << "Starting pathfinding..." << endl;
			}

			dijkstraPathfinding(start, end, vertices, edges);
			
			if (DEBUG) {
				cout << "Finished pathfinding" << endl;
			}

			continue;

		}	

		if (tokens.front() == "q") {
			continue;
		}

		//no matching command
		cout << "No matching command" << endl;


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

void dijkstraPathfinding(char start, char target, set<char> vertices, set<edge> edges) {

	if (DEBUG) {
		cout << "Creating lists and table..." << endl;
	}

	//set queues up and create map to track shortest paths
	set<char> visited;
	set<char> unvisited;	
	map<char, tuple<double, char>> paths;

	if (DEBUG) {
		cout << "Adding vertices to lists and table..." << endl;
	}

	for (char v : vertices) {

		//add all vertices to unvisited
		unvisited.insert(v);

		//add all vertices to map
		//path length is 0 if the vertex is the start
		//otherwise max it out
		paths[v] = make_tuple((v == start ? 0 : numeric_limits<double>::max()), ' ');
	}

	if (DEBUG) {
		cout << "Starting algorithm..." << endl;
	}

	char current = start;

	//begin algorithm
	while (!unvisited.empty()) {

		unvisited.erase(current);

		if (DEBUG) {
			cout << "At vertex " << current << endl;
			cout << "Getting neighbors..." << endl;
		}

		//get unvisited neighbors
		queue<edge> neighbors;
		for (edge e : edges) {
			//unvisited neighbor if:
			// - the edge starts at the current vertex
			// - the edge ends at an unvisited vertex 
			if (e.start == current && unvisited.count(e.end)) {
				neighbors.push(e);

				if (DEBUG) {
					cout << "Found unvisited neighbor " << e.end << endl;
				}
			}
		}	

		if (DEBUG) {
			cout << "Calculating path lengths..." << endl;
		}

		//for each neighbor, check and update paths
		while (!neighbors.empty()) {
			//store the current neighbor
			edge neighbor = neighbors.front();
			neighbors.pop();

			//calculate the path length
			double length = get<0>(paths[current]) + neighbor.value;

			if (DEBUG) {
				cout << "Path from start to " << neighbor.end << " is " << length << endl;
			}

			//store length if shortest
			if (length < get<0>(paths[neighbor.end])) {
				paths[neighbor.end] = make_tuple(length, current);
			}

		}

		if (DEBUG) {
			cout << "Picking next vertex..." << endl;
		}

		//go to the unvisited vertex with the shortest current path
		//add current vertex to visited
		visited.insert(current);

		char shortest = ' ';
		for (char v : unvisited) {
			
			//check for new shortest
			if (shortest == ' ' || get<0>(paths[v]) < get<0>(paths[shortest])) {
				shortest = v;
			}			
		}
	
		current = shortest;

		if (DEBUG && !unvisited.empty()) {
			cout << "Continuing to next vertex..." << endl;
		}
	}

	if (DEBUG) {
		cout << "Isolating shortest path..." << endl;
	}

	//get results
	
	//if the shortest path length is still max, then there is no path
	if (get<0>(paths[target]) == numeric_limits<double>::max()) {
		cout << "No connecting path" << endl;
		return;
	}

	stack<char> shortestPath;
	current = target;
	
	//backtrack through the table to see which path leads to the target
	while (current != start) {
		current = get<1>(paths[current]);
		shortestPath.push(current);
	}

	cout << "Shortest path: ";
	while (!shortestPath.empty()) {
		cout << shortestPath.top() << " -> ";
		shortestPath.pop();
	}

	cout << target << endl; 
	
	cout << "Shortest path length: " << get<0>(paths[target]) << endl;

}








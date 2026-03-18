/*
   A text-based graph creator with an adjacency table visualization and Dijkstra's algorithm.

Author: Luca Ardanaz
 */

#include <iostream>
#include <string>
#include <vector>
#include "Vertex.h"

using namespace std;

string toLower(string str);

int main() {

	string input = "";
	vector<Vertex*> vertices;

	do {

		// --<< GET USER INPUT >>--
		cout << endl << endl;
		cout << "Enter command (enter 'help' for commands):" << endl;
		getline(cin, input);
		input = toLower(input); 

		// --<< RUN CORRESPONDING COMMAND >>--

		if (input == "help") {
			cout << endl;
			cout << "Commands:" << endl;
			cout << "av - add vertex" << endl;
			cout << "ae - add edge" << endl;
			cout << "rv - remove vertex" << endl;
			cout << "re - remove edge" << endl;
			cout << "p - print adjacency table" << endl;
			cout << "f - find shortext path" << endl;
			cout << "q - quit" << endl;
			continue;
		}			

		if (input == "av") {

			//ask for the label
			cout << endl << "Enter unique label (single character):" << endl;
			string labelStr;
			getline(cin, labelStr);
			char label = labelStr[0];
	
			//confirm that the label is unique
			bool unique = true;
			for (Vertex* v : vertices) {
				unique = v -> getLabel() == label ? false : unique;
			}

			if (!unique) {
				cout << "That label is already taken" << endl;
				continue;
			}

			//add the vertex
			vertices.push_back(new Vertex(label));

			continue;
		}	

		if (input == "p") {
			
			if (!vertices.empty()) {

			//create empty table
			bool table[vertices.size()][vertices.size()] = {false};
			
			//determine values

			//print table
			
			}

			else {
				cout << "Add vertices first" << endl;
			}
	
			continue;
		}	

	} while (input != "q");	

	return 0;
}

string toLower(string str) {
	for (char& c : str) {
		c = (char)tolower(c);
	}

	return str;
}

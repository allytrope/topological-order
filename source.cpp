#include <string>
#include <vector>
#include <map>
#include <queue>
#include <iostream>
using namespace std;

/*
This program finds the topological order for the vertices of graphs using DFS and BFS.
*/

class graph {
public:
	vector <vector <int>> adjList = {}; //indexed by vertices
	map <string, int> nameMap;
	vector <string> vertexNames;
	int order;
	graph(vector <vector <string>> oriAdjList, vector <string> vertexNames) {
		this->vertexNames = vertexNames;
		//creates map for string names into integers from 0 o n-1
		for (unsigned int i = 0; i < vertexNames.size(); i++) {
			string name = vertexNames[i];
			nameMap[name] = i;
		}
		//creates adjacency list from map
		for (unsigned int i = 0; i < oriAdjList.size(); i++) {
			adjList.push_back({}); //sets size of adjList
			for (unsigned int j = 0; j < oriAdjList[i].size(); j++) {
				adjList[i].push_back({}); //sets each subsize of adjList
				string name = oriAdjList[i][j];
				adjList[i][j] = nameMap[name];
			}
		}
		order = oriAdjList.size();
	}
};

//called by DFS1
int DFS2(graph* G, int vertex, vector <int> *reverseOrder, vector <bool> *hasBeenOrdered, vector <bool> *isAncestor) {
	//iterates through outedges of vertex
	for (auto it = G->adjList[vertex].begin(); it != G->adjList[vertex].end(); it++) {
		//skips one iteration if already ordered
		if (hasBeenOrdered->at(*it) == true) {
			continue;
		}
		//for detecting cycle
		else if (isAncestor->at(*it) == true) {
			//passes error to DFS1()
			cout << "Cycle detected!!" << endl;
			return -1;
		}
		else {
			isAncestor->at(*it) = true;
			int status = DFS2(G, *it, reverseOrder, hasBeenOrdered, isAncestor);
			//continues error when cycle is present
			if (status == -1) {
				return -1;
			}
			isAncestor->at(*it) = false;
		}
	}
	reverseOrder->push_back(vertex);
	hasBeenOrdered->at(vertex) = true;
	return 0;
}

int DFS1(graph* G) {
	vector <int> reverseOrder; //reverse topological ordering
	vector <bool> isAncestor; //true if ancestor of current vertex during one DFS
	vector <bool> hasBeenOrdered; //true if given a topological order
	for (unsigned int i = 0; i < G->adjList.size(); i++) {
		isAncestor.push_back(false);
		hasBeenOrdered.push_back(false);
	}
	//iterates through vertices
	for (unsigned int i = 0; i < G->adjList.size(); i++) {
		//only performs DFS if not yet visited
		if (hasBeenOrdered[i] == false) {
			isAncestor[i] = true;
			int status = DFS2(G, i, &reverseOrder, &hasBeenOrdered, &isAncestor);
			if (status == -1) {
				return -1;
			}
			isAncestor[i] = false;
		}
		hasBeenOrdered[i] = true;
	}
	//prints topological order
	cout << "Topological order:" << endl;
	int order = 1;
	for (auto it = reverseOrder.rbegin(); it != reverseOrder.rend(); it++) {
		string j = G->vertexNames[*it];
		cout << order << ": " << j << endl;
		order++;
	}
	return 0;
}


void BFS(graph G) {
	//finds vertices with no in-edges
	vector <int> predCount;
	predCount.resize(G.vertexNames.size());
	//sets all values to false
	for (unsigned int i = 0; i < G.adjList.size(); i++) {
		predCount[i] = 0;
	}
	for (unsigned int i = 0; i < G.adjList.size(); i++) {
		//finds which vertices have parents
		for (auto it = G.adjList[i].begin(); it != G.adjList[i].end(); it++) {
			predCount[*it]++;
		}
	}

	queue <int> vertQueue;
	vector <int> topoOrder = {};
	//starting queue with parentless vertices
	for (unsigned int i = 0; i < predCount.size(); i++) {
		if (predCount[i] == 0) {
			vertQueue.push(i);
		}
	}
	while (!vertQueue.empty()) {
		topoOrder.push_back(vertQueue.front());
		vector <int> temp = {}; //used for keeping track of which vertices lose parents in each cycle
		for (auto it = G.adjList[vertQueue.front()].begin(); it != G.adjList[vertQueue.front()].end(); it++) {
			predCount[*it]--;
			temp.push_back(*it);
		}
		vertQueue.pop();
		for (auto it = temp.begin(); it != temp.end(); it++) {
			if (predCount[*it] == 0) {
				vertQueue.push(*it);
			}
		}
	}
	//checks for cycle
	if (topoOrder.size() < G.vertexNames.size()) {
		cout << "Cycle detected!!" << endl;
		return;
	}
	//prints topological order
	cout << "Topological order:" << endl;
	int order = 1;
	for (auto it = topoOrder.begin(); it != topoOrder.end(); it++) {
		string j = G.vertexNames[*it];
		cout << order << ": " << j << endl;
		order++;
	}
	return;
}


int main() {
	/*
	Input the names of vertices under "vertexNames#" and the adjacency list for the corresponding vertices in "adjList#"
	*/


	vector <string> vertexNames1 = { "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };
	vector <vector <string>> adjList1 = {
		{"q", "r", "x"}, //m
		{"o", "q", "u"}, //n
		{"r", "s"}, //o
		{"o", "s", "z"}, //p
		{"t"}, //q
		{"u", "y"}, //r
		{"r"}, //s
		{}, //t
		{"t"}, //u
		{"w", "x"}, //v
		{"z"}, //w
		{}, //x
		{"v"}, //y 
		{}, //z
	};
	cout << "Graph 1" << endl;
	cout << "DFS" << endl;
	graph graph1(adjList1, vertexNames1);
	DFS1(&graph1);
	cout << endl << "BFS" << endl;
	BFS(graph1);
	cout << endl;

	vector <string> vertexNames2 = { "1", "2", "3", "4", "5", "6", "7", "8" };
	vector <vector <string>> adjList2 = {
		{"2", "5", "6"}, //1
		{"3", "5", "7"}, //2
		{"4"}, //3
		{"5"}, //4
		{"7", "8"}, //5
		{"5", "8"}, //6
		{"4", "8"}, //7
		{} //8
	};
	cout << "Graph 2" << endl;
	cout << "DFS" << endl;
	graph graph2(adjList2, vertexNames2);
	DFS1(&graph2);
	cout << endl << "BFS" << endl;
	BFS(graph2);

	return 0;
}

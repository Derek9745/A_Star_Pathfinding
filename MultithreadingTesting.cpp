// MultithreadingTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
     
     
#include <vector>
#include <queue>
#include <utility>
#include <iostream>
#include <climits>

using namespace std;




//Node class
class Node {
	int vertex;   // which node it is
	int distance; // current distance from start node
	int fcost; //estimated cost of going through a node ( distance + heuristic)
	int x;
	int y;

public:
	Node(int v, int d, int f, int x_coord =0, int y_coord=0) {
		vertex = v;
		distance = d;
		fcost = f;
		x = x_coord;
		y = y_coord;
	}

	int getX() const { 
		return x; 
	}

	int getY() const { 
		return y; 
	}

	int getCost() const {
		return fcost;
	}

	int getDistance() const {
		return distance;
	}

	int getVertex() const {
		return vertex;
	}

	struct CompareNode {
		bool operator()(const Node& a, const Node& b) {
			return a.getCost() > b.getCost(); // min-heap
		}
	};
};

//Graph class
class Graph {
	int v;
	vector<vector<pair<int, int>>> adjList;
	vector<Node>nodeList;

public:
	Graph(const vector<vector<pair<int, int>>>& adjList, vector<Node>nodeList) {
		this->adjList = adjList;
		this->v = adjList.size();
		this->nodeList = nodeList;
	}

	int getVector() const {
		return v;

	}

	const vector<Node>& getNodeList() const {
		return nodeList;

	}

	const vector<vector<pair<int, int>>>& getAdjacent() const {
		return adjList;

	}

	//insert an edge into the graph
	void InsertEdge(int from, int to, int weight) {
		adjList[from].push_back({ to, weight });

	}

	//add a new graph into dijkstra
	void CreateGraph(const vector<vector<pair<int, int>>>& newAdj) {
		adjList = newAdj;
		v = newAdj.size();
	}

	//remove the current graph from the a* function
	void DestroyGraph() {
		adjList.clear();
		v = 0;
	}

	//for loop to iterate through the graph and print all the nodes in the graph
	void PrintGraph() {
		for (int i = 0; i < adjList.size(); i++) {
			cout << "vertex: " << i << endl;
			for (int j = 0; j < adjList[i].size(); j++) {
				cout <<"neighbor vertex: " <<  adjList[i][j].first << ", weight: " << adjList[i][j].second << endl;
			}

		}
	}

	int heuristic(const Node& a, const Node& b) {
		return abs(a.getX() - b.getX()) + abs(a.getY() - b.getY());
	}
};



struct AStarResult {
	vector<int> distances;
	vector<int> shortestPath;
};


static AStarResult AStar(Graph& graph, Node& startNode,Node& goalNode) {
	priority_queue<Node, vector<Node>, Node::CompareNode>pq;
	auto& nodeList = graph.getNodeList(); 
	vector<int> distance(graph.getVector(), INT_MAX);
	vector<int> shortestPath(graph.getVector(), -1);
	
	distance[startNode.getVertex()] = 0;

	pq.push(startNode);

	while (!pq.empty()) {
		//remove vertex with smallest distance from the queue
		Node top = pq.top();
		pq.pop();

		int d = top.getDistance();
		int u = top.getVertex();


		if (u == goalNode.getVertex()) break;

		//Check: if the popped distance is greater than the recorded distance for this vertex(dist[u]),
		// it means this vertex has already been processed with a smaller distance, 
		// so skip it and continue to the next iteration.
		if (d > distance[u]) {
			continue;
		}

		//explore all adjacent neighbors of current vertex
		for (int i = 0; i < graph.getAdjacent()[u].size(); i++) {
			pair<int, int> p = graph.getAdjacent()[u][i];

			//adjacent vertex
			int v = p.first;
			//adjacent edge?
			int w = p.second;

			//foreach neighbor v of u, check if the path through u gives a smaller distance thatn current dist v
			if (distance[u] + w < distance[v]) {
				//if it does, update dist[v] = dist[u]
				distance[v] = distance[u] + w;
				shortestPath[v] = u;

				int f = distance[v] + graph.heuristic(nodeList[v], goalNode);
				pq.push(Node(v, distance[v], f, nodeList[v].getX(), nodeList[v].getY()));

			}
		}

	}

	//return final shortest distance	
	return { distance, shortestPath };
};

// Function to print the actual path from start to a given goal
void PrintPath(int goal, const vector<int>& parents) {
	vector<int> path;
	int current = goal;
	while (current != -1) {
		path.push_back(current);
		current = parents[current]; // move to parent
	}
	// reverse to get path from start → goal
	reverse(path.begin(), path.end());

	cout << "Actual path from start to goal: ";
	for (int i = 0; i < path.size(); i++) {
		cout << path[i];
		if (i != path.size() - 1) cout << " -> ";
	}
	cout << endl;
}

int main() {
	int maxInt = INT_MAX; 

	vector<Node> nodeList = {
	Node(0, 0, 0, 0, 0),   // Node 0 at (0,0), start node distance 0
	Node(1, INT_MAX, INT_MAX, 1, 2),  // Node 1 at (1,2)
	Node(2, INT_MAX, INT_MAX, 2, 0),  // Node 2 at (2,0)
	Node(3, INT_MAX, INT_MAX, 3, 2),  // Node 3 at (3,2), goal node
	Node(4, INT_MAX, INT_MAX, 4, 0)   // Node 4 at (4,0), optional extra node
	};

	// Define start and goal nodes by referencing nodeList directly
	Node& startNode = nodeList[0];
	Node& goalNode = nodeList[3];

	vector<vector<pair<int, int>>> adj(5); // 5 vertices, empty edges


	Graph graph(adj,nodeList);

	graph.InsertEdge(0, 1, 4);
	graph.InsertEdge(0, 2, 2);
	graph.InsertEdge(1, 3, 1);
	graph.InsertEdge(2, 3, 5);


	AStarResult result = AStar(graph, startNode,goalNode);

	graph.PrintGraph();

	for (int i = 0; i <result.distances.size(); i++) {
		if (result.distances[i] == maxInt) {
			cout << "Shortest Distance = infinity representation" << endl;
		}
		else {
			cout << "Shortest Distance:" << result.distances[i] << endl;
		}
		
	}

	//Print shortestPath array(parent nodes)
	cout << "\nShortest Path:" << endl;
	for (int i = 0; i < result.shortestPath.size(); i++) {
		cout << "Node " << i << " parent: " << result.shortestPath[i] << endl;
	}

	PrintPath(goalNode.getVertex(), result.shortestPath);


	return 0;
};






// Code by Aljon Viray (86285526)
// ICS 46 - Homework #8 - kruskals.cpp
// Kruskal's Algorithm

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;


/////////////////////  Classes  /////////////////////


class Edge
{
	public:
		int src;
		int dst;
		int weight;

		Edge (int s = -1, int d = -1, int w = -1)
			: src{s}, dst{d}, weight{w} {}

		Edge reverse() //Recommended, not required
			{ return *this; }

		void print (ostream& out)
			{ cout << src << "---" << dst << " with weight = " << weight << endl; }

		int get_weight() 
			{ return weight; }
};


class Vertex
{
	public:
		int id; 
		vector<Edge> edges;

		Vertex (int i = -1) // -1 meaning not a vertex that exists
			{ id = i; }

		void add_edge (const Edge& e)
			{ edges.insert(edges.begin(), e); }

		void print (ostream& out)
		{
			out << "vector id = " << id << endl;
			out << "edges: " << endl;
			for (int i = 0; i < edges.size(); i++)
				edges[i].print(out);
		}
};


class Graph
{
	public:
		Vertex* vertices; // Array of vertices
		int num_vertex;
		int num_edge;

		Graph (string file_name)
		{
			ifstream infile(file_name);
			infile >> num_vertex; // first input is num_vertex
			vertices = new Vertex[num_vertex];

			int src;   int dst;   int weight;
			while (infile >> src && infile >> dst && infile >> weight)
			{
				//cout << src << ' ' << dst << ' ' << weight << endl;
				add_edge(src, dst, weight);
			}
		}

		void add_edge (int src, int dst, int weight)
		{
			Edge e (src, dst, weight);

			// Create vertices if they do not exist (-1 means it does not exist yet)
			if (vertices[src].id == -1) vertices[src].id = src;
			if (vertices[dst].id == -1) vertices[dst].id = dst;

			// Connect edges to the vertices (both vertices get an edge)
			vertices[src].add_edge(e);
			num_edge++; // We pretend both edges are the same here bc undirected
		}

		void print (ostream& out)
		{
			for (int i = 0; i < num_vertex; i++)
				vertices[i].print(out);
		}

		~Graph()
			{ delete[] vertices; }
};


//////////////////////  Sets Functions  ///////////////////////////


struct SetNode
{
	SetNode* parent;
	int id;  int rank;  int size;

	SetNode ()
		: parent{this}, id{0}, rank{0}, size{0} {}
};


struct Set
{
	int count;
	SetNode* elements;

	Set (int N) 
		: count{N}, elements{new SetNode[N]} {}

	void error (const char* msg) // O(1)
	{
		cerr << "ERROR" << msg << endl;
		exit(-1);
	}

	void verify_set_id (int x) // O(1)
	{
		if (x < 0 || x >= count)
			error("ID out of bounds");
	}

	void make_set (int x) // O(1)
	{
		verify_set_id(x);
		SetNode& my_node = elements[x];

		my_node.parent = &my_node;
		my_node.rank = 0;
		my_node.size = 1;
	}

	SetNode* find (SetNode* x) // O(1)
	{
		if (x->parent != x)
			x->parent = find(x->parent);
		return x->parent;
	}

	SetNode* find (int x) // O(1)
	{
		verify_set_id(x);
		SetNode& my_node = elements[x];
		return find(&my_node);
	}

	void union_nodes (int x, int y) // O(1)
	{
		SetNode* xRoot = find(x);
		SetNode* yRoot = find(y);
		if (xRoot == yRoot)
			return;
		if (xRoot->size < yRoot->size)
			swap(xRoot, yRoot);
		yRoot->parent = xRoot;
		xRoot->size = xRoot->size + yRoot->size;
	}

	~Set()
		{ delete[] elements; }
};


///////////////////  Kruskals Function  ////////////////////////

class CompareClass
{
	public:
		bool operator() (const Edge& e1, const Edge& e2) // O(1)
			{ return (e1.weight > e2.weight); } // Causes sort from least to greatest weight
};


vector<Edge> kruskals (const Graph& g) // O(|V| log |E|)
{
	
	int N = g.num_vertex;
	Set S(N);
	priority_queue<Edge, vector<Edge>, CompareClass> Q;
	vector<Edge> T;

	for (int i = 0; i < g.num_vertex; i++)
	{
		S.make_set(g.vertices[i].id);
		for (int j = 0; j < g.vertices[i].edges.size(); j++)
			Q.push(g.vertices[i].edges[j]);
	}

	for (int i = 0; T.size() < N-1; i++)
	{
		Edge e = Q.top(); // Top is the smallest weighted Edge
		if (S.find(e.src) != S.find(e.dst)) // If not in the same Set
		{
			T.push_back(e); // Add Edge e to solution
			S.union_nodes( e.src, e.dst ); // Join the two subtrees into one tree
		}
		Q.pop();
	}
	return T;
}


void printMST (vector<Edge> results)
{
	// Print total cost of the MST
	int total = 0;
	for (int i = 0; i < results.size(); i++)
		total += results[i].weight;
	cout << total << endl;

	// Print every edge in the MST
	for (int i = 0; i < results.size(); i++)
		cout << '[' << results[i].src << '-' << results[i].dst << "] (" << results[i].weight << ')' << endl;
}


/////////////////////  Main Function  /////////////////////////


int main (int argc, char* argv[])
{
	cout << "----------------------------------" << endl;
	
	const char* file = argc == 2 ? argv[1] : "small.graph";
	//const char* file = argc == 2 ? argv[1] : "large.graph";
	//const char* file = argc == 2 ? argv[1] : "rdm.graph";

	Graph g(file);
	//g.print(cout);
	vector<Edge> results = kruskals(g);
	printMST(results);
	
	cout << "----------------------------------" << endl;

	return 0;
}

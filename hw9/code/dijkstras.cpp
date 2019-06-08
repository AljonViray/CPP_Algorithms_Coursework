// Code by Aljon Viray (86285526)
// ICS 46 - Homework #9 - dijkstras.cpp
// Dijkstra's Algorithm

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <climits>
#include <algorithm>
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

		void print (ostream& out)
			{ cout << src << "---" << dst << " with weight = " << weight << endl; }
};


class Vertex
{
	public:
		int id; 
		vector<Edge> edges;

		Vertex (int i = -1) // -1 meaning not a vertex that exists
			{ id = i; }

		void operator= (Vertex v)
		{
			id = v.id;
			edges = v.edges;
		}

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
			infile >> num_vertex; // First input is num_vertex
			vertices = new Vertex[num_vertex];
			for (int i = 0; i < num_vertex; i++)
				vertices[i].id = i;
			int src;   int dst;   int weight;
			while (infile >> src && infile >> dst && infile >> weight)
				add_edge(src, dst, weight);
		}

		void add_edge (int src, int dst, int weight)
		{
			Edge e (src, dst, weight);
			if (vertices[src].id == -1) vertices[src].id = src; // Create vertices if they do not exist 
			if (vertices[dst].id == -1) vertices[dst].id = dst;
			vertices[src].add_edge(e);
			num_edge++;
		}

		void print (ostream& out)
			{ for (int i = 0; i < num_vertex; i++) vertices[i].print(out); }

		~Graph()
			{ delete[] vertices; }
};


//////////////////////////////////////////////////////


class HeapSorter
{
	public:
		struct Node 
		{ 
			Vertex vertex; 
			int distance; 
		};
		Node* buf;
		int capacity;
		int size;

		HeapSorter (int N)
			: buf{new Node[N]}, capacity{N}, size{0} {}

		int leftChild (int i)
			{ return 2*i+1; }

		int rightChild (int i)
			{ return 2*i+2; }

		int parent (int i)
			{ return (i-1)/2; }

		void siftLargestUp (int i) // O(1)
		{
			int p = parent(i);
			while (i != 0 && buf[p].distance > buf[i].distance) { // While parent value is greater than its children
				swap(buf[p], buf[i]);
				p = parent(p);
			}
		}

		void setDistance (int v, int d) // O(N)
		{
			for (int i = 0; i < size; i++)
			{
				if (buf[i].vertex.id == v)
				{
					buf[i].distance = d;
					siftLargestUp(i);
					break;	
				}
			}
		}
		
		void minHeapify (int i) // O(N)
		{
			int left = leftChild(i);
			int right = rightChild(i);
			int smallest = i;

			if (left < size && buf[left].distance < buf[i].distance)
				smallest = left;
			if (right < size && buf[right].distance < buf[smallest].distance)
				smallest = right;
			if (smallest != i)
			{
				swap(buf[i], buf[smallest]);
				minHeapify(smallest);
			}
		}

        void heapSort() // O(N)
		{ 
			for (int i = size/2-1; i >= 0; i--) 
				minHeapify(i); 		  
			for (int i = size-1; i >= 0; i--) 
			{ 
				swap(buf[0], buf[i]);  
				minHeapify(i); 
			} 
		}
};


/////////////////////////////////////////////////////


class PriorityQueue : public HeapSorter
{
	public:
		using HeapSorter::HeapSorter;

		void enqueue (Vertex v, int weight) // O(1)
		{
			buf[size].vertex = v;
			buf[size].distance = weight;
			size++;
		}

		Vertex dequeue() // O(N)
		{
			Vertex v = buf[0].vertex;
			size--;
			buf[0].vertex = buf[size].vertex;
			buf[0].distance = buf[size].distance;
			heapSort();
			return v; 
		}

		void print()
			{ for (int i = 0; i < capacity; i++) cout << i << ": " << buf[i].vertex.id << endl; }

		bool isEmpty()
			{ return size == 0; }

		~PriorityQueue()
			{ delete[] buf; }
};


///////////////////  Dijkstras Function  ////////////////////////


void printPathOnly (int* prev, int j)
{
	if (prev[j] == -1) // Base Case: j == start
		return;
	printPathOnly(prev, prev[j]);
	cout << "->" << j;
}

void printDijkstras (int* dist, int* prev, const Graph& g, int start) // O(N)
{
	for (int i = 0; i < g.num_vertex; i++)
	{
		cout << i << " [" << start;
		printPathOnly(prev, i);
		cout << "] (" << dist[i] << ")" << endl; 
	}
}

void dijkstras (const Graph& g, int start) // O( (|E|+|V|) log |V|)
{
	int dist[g.num_vertex];
	int prev[g.num_vertex];
	PriorityQueue Q(g.num_vertex);

	for (int i = 0; i < g.num_vertex; i++) { // Initialize the arrays
		dist[i] = INT_MAX;	// "Infinity"
		prev[i] = -1;		// "Undefined"
		Q.enqueue(g.vertices[i], dist[i]);
	}
	
	dist[start] = 0;	// Distance from start to start = 0
	Q.setDistance(start, 0);

	while (!Q.isEmpty()) 
	{
	 	Vertex u = Q.dequeue();
		for (int i = 0; i < u.edges.size(); i++) // For each edge of the min vertex u,
		{													 
			Edge e = u.edges[i];
			Vertex v = g.vertices[u.edges[i].dst];
			if (dist[v.id] > (dist[u.id] + e.weight)) 
			{
				dist[v.id] = dist[u.id] + e.weight; // The distance from source to v is saved at dist[v]
				prev[v.id] = u.id;
				Q.setDistance(v.id, dist[v.id]);
			}
		}
	}
	printDijkstras(dist, prev, g, start);
}


/////////////////////  Main Function  /////////////////////////


int main (int argc, char* argv[])
{
	cout << "----------------------------------" << endl;
	
	const char* start = argc == 3 ? argv[1] : "0"; // Get the vertex we start Dijkstras Algorithm on
	stringstream s(start);
	int startVertex = 0;
	s >> startVertex;   // Convert string to int

	const char* file = argc == 3 ? argv[2] : "small.graph";
	//const char* file = argc == 2 ? argv[1] : "large.graph";
	//const char* file = argc == 2 ? argv[1] : "rdm.graph";

	Graph g(file);
	//g.print(cout);
	dijkstras(g, startVertex);
	
	cout << "----------------------------------" << endl;

	return 0;
}

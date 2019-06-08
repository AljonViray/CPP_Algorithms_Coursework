#include <iostream>
#include <stdlib.h>

using namespace std;

void error(char * msg)
{
    cerr << "Error: " << msg << endl;
    exit(-1);
}

int main(int argc, char *argv[])
{
    srandom(0);
    const int NumEdges = 4;
    const int NumVertices = 1024;
    cout << NumVertices << endl;
    for (int i=0; i < NumVertices; ++i)
    {
        for (int j=1; j <= NumEdges; ++j)
        {
            int cost = 1 + random() % 19;
            cout << i << " " << (i+j) % NumVertices << " " << cost << endl;
        }
    }
    return 0;
}

// 	EXAMPLE USE (in terminal)
//	$ g++ -std=c++11 genGraph.cpp -o genGraph
//	$ genGraph > rdm.graph
//  Your random graph is output to rdm.graph

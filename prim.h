//Prim's MST Algorithm Header File
//#ifndef PRIM
//#define PRIM
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <ctime>
#define INF 0x3f3f3f3f
using namespace std;

typedef pair<int, int> iPair;
/*
    pair<> - stores adjacent node number and weight of edge connection
    adj[] - adjacency list representation of weighted graph
    vector<> - stores adjacency lists of all nodes
*/

class Prim{
    private:
    vector<int> MST; //minimum spanning tree
    int n; //length and width of square graph/maze
    int SIZE; //graph size (number of nodes)

    public:
    Prim():MST(0), n(0), SIZE(0){}
    Prim(int n, int SIZE){this->n = n; this->SIZE = SIZE;}
    void addEdge(vector<pair<int, int> > adj[], int u, int v, int wt);
    void primMST(vector<pair<int,int> > adj[]);
    vector<int> Print(); //Prints new graph/maze's open paths/edges
};

//#endif
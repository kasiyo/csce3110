//Prim's MST Algorithm CPP File
#include "prim.h"
using namespace std;

void Prim::addEdge(vector <pair<int, int> > adj[], int u, int v, int wt){
	adj[u].push_back(make_pair(v, wt));
	adj[v].push_back(make_pair(u, wt));
}

void Prim::primMST(vector<pair<int,int> > adj[]){
	//Priority queue to store nodes and to implement heap
    //priority_queue <Type, vector<Type>, ComparisonType > min_heap;
	priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

	int src = 0; //node 0 as source

    //Initialize keys(weight of paths/edges) of all nodes 
    //as infinite (INF) and parent of every node as -1
	vector<int> key(SIZE, INF);
	vector<int> parent(SIZE, -1); //stores MST as well

	//Initialize all nodes as not part of MST yet
	vector<bool> inMST(SIZE, false);

	//Insert source and initialize its key as 0
	pq.push(make_pair(0, src)); key[src] = 0;

	while (!pq.empty()){
		//Extract minimum key node from pq
		int u = pq.top().second;
		pq.pop();

		if(inMST[u] == true){continue;}
		inMST[u] = true; //Include node in MST

		//Traverse all adjacent of u
		for(auto x : adj[u]){
			//Get node index and weight of current adjacent
			int v = x.first;
			int weight = x.second;

			//If v is not in MST and weight of (u,v)
			//is smaller than current key of v
			if(inMST[v] == false && key[v] > weight){
				//Update v key
				key[v] = weight;
				pq.push(make_pair(key[v], v));
				parent[v] = u;
			}
		}
	}
    MST = parent;
}

vector<int> Prim::Print(){
    //Print edges of MST
    vector<iPair > adj[SIZE];
	srand(time(0));
	
	for(int i = 0; i < SIZE; i++){
	    
        //Randomize the weights of each possible path
	    int north = rand() % 10 + 1;
	    int south = rand() % 10 + 1;
	    int east = rand() % 10 + 1;
	    int west = rand() % 10 + 1;
	    
	    if((i - n) >= 0){ addEdge(adj, i, i - n, north); }
	    if((i + n) < SIZE){ addEdge(adj, i, i + n, south); }
	    
	    if((i % n) != (n - 1)){ //if not on the right border
	        if((i + 1) < SIZE){ addEdge(adj, i, i + 1, east); }
	    }
	    if((i % n) != 0){ //if not on the left border
	        if((i - 1) >= 0){ addEdge(adj, i, i - 1, west); }
	    }
	}

	primMST(adj);
    printf("\n_____List of Open Edges in Maze_____\n");
	for(int i = 1; i < SIZE; ++i){ printf("\t\t%d - %d\n", MST[i], i); }
	return MST;
}

//A* Algorithm Header File
#include <iostream>
#include <set>
#include <stack>
#include <cmath>
#include <cstdio>
#include <utility>
#define FLOAT_MAX 99999999999999.99
#define MAX_ROW 9
#define MAX_COL 9
using namespace std;

struct node{ //stores cost of nodes and parents of nodes for stack
	unsigned int parent_i;
	unsigned int parent_j;
	/*
	Row and Column index of its parent
    0 <= i <= ROW-1 & 0 <= j <= COL-1
	*/

	double f, g, h;
	/* 
	f = g + h, each step A* picks the node with the lowest f
	g = movement cost to move from the starting point to a given square on the grid 
	h = estimated movement cost to move from that given square on the grid 
		to the final destination (aka the heuristic)

	The type of heuristic used in this A* is the Manhattan Distance, since we can 
	only move in 4 directions -> north, south, east, west.
	where h = abs (current_cell.x – goal.x) + abs (current_cell.y – goal.y)
	*/
	node(): parent_i(-1), parent_j(-1), f(FLOAT_MAX), g(FLOAT_MAX), h(FLOAT_MAX){}
};

typedef pair<int, int> Pair; //stores coordinates
typedef pair<double, pair<int, int> > pPair; //stores cost and coordinates

class Astar{
	public:
	double getHeuristic(int x, int y, int destX, int destY); 
	//gets Manhattan Distance between source x/y destination x/y

	bool nodeBoundCheck(int x, int y); //check if node is out of range
	bool nodeBlockCheck(int x, int y, int grid[][MAX_COL]); //check if node is in wall

	bool destReached(int x, int y, int destX, int destY); //check if destination is reached

	void tracePath(node details[][MAX_COL], Pair dest); //test output
	//dest = destination
	//details = node details

	void generatePath(pair<int, int> source, pair<int, int> dest, int grid[][MAX_COL]);
	//generate path between source and destination 

};

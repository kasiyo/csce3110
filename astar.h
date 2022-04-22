#include <iostream>
#include <cmath>
#include <bits/stdc++.h>
#include <cstdio>
#include <utility>
#define FLOAT_MAX 99999999999999.99
#define MAX_ROW 9
#define MAX_COL 10
using namespace std;

struct node{
	unsigned int parent_i;
	unsigned int parent_j;
	double f, g, h;
	node(): parent_i(-1), parent_j(-1), f(FLOAT_MAX), g(FLOAT_MAX), h(FLOAT_MAX){}
};

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

class Astar{
	public:
	double getEuclideanDistance(int x, int y);
	double getHeuristic(int x, int y, int destX, int destY);

	bool nodeBoundCheck(int x, int y);
	bool nodeBlockCheck(int x, int y, int grid[][MAX_COL]);

	bool destReached(int x, int y, int destX, int destY);

	void tracePath(node details[][MAX_COL], Pair dest);

	void generatePath(pair<int, int> source, pair<int, int> dest, int grid[][MAX_COL]);
};

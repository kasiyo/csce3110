#include <iostream>
#include <stdio.h>
#include "astar.h"
using namespace std;

int main(){
	int grid[MAX_ROW][MAX_COL] = {
    { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
    { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
    { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
    { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
    { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
    { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }
    };

	pair<int, int> source = make_pair(8, 0);
	pair<int, int> dest = make_pair(0, 0);
	Astar newAstar;
	newAstar.generatePath(source, dest, grid);

	return 0;
}
#include <iostream>
#include <stdio.h>
#include "astar.h"
using namespace std;

int main(){
	int grid[MAX_ROW][MAX_COL] = {
    { 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
    { 0, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
    { 0, 0, 1, 0, 1, 0, 0, 1, 0, 1 },
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 0 },
    { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
    { 1, 1, 0, 0, 0, 1, 0, 0, 0, 1 },
    { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }
    };

    //note: x is actually the y axis and y is the x axis if your looking at the
    //above grid representation as a cartesean plane
	pair<int, int> source = make_pair(8, 0); //Source is the left-most bottom-most corner
	pair<int, int> dest = make_pair(0, 0); //Destination is the left-most top-most corner
	Astar newAstar;
	newAstar.generatePath(source, dest, grid);

	return 0;
}
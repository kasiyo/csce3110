//Main CPP File
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include <vector>
#include "astar.h"
#include "prim.h"
using namespace std;

int main(){
    int n = 5; //length and width of square graph/maze
    int SIZE = pow(n, 2);
	Prim prim(n, SIZE);
    vector<int> mst; ////minimum spanning tree
	mst = prim.Print(); //Prints new graph/maze's open paths/edges

    int s = 2*n - 1; //grid size = 9
    int grid[9][9]; //must be hardcoded due to passing constraints
    //hard coded here and in astar.h for definitions of MAX_ROW and MAX_COl
    //if you want to pass variable sizes, you would need to change
    //the 2D grid arrays to 2D vectors.

    memset(grid, 0, sizeof(grid)); //set grid to all 0's
    //random maze will be filled with 1's and 0's
    //1's are open paths, 0's are closed paths
    //nodes are separated by a 1 or 0, where a 1 means it's
    //connected to another node and part of an open path

    
    //Filling grid with open paths
    int x_1, y_1, x_2, y_2;
    for(int i = 1; i < SIZE; ++i){
        x_1 = (i % n) * 2;
        y_1 = (i / n) * 2;
        grid[x_1][y_1] = 1;

        x_2 = (mst[i] % n) * 2;
        y_2 = (mst[i] / n) * 2;
        grid[x_2][y_2] = 1;

        //Finding the middle open "path"/edge
        //connects the two nodes together...
        //Note: x is actually the y axis and y is
        //the x axis if you're imagining the grid
        //as a cartesean plane. Remember that each
        //row will output its column values.

        //First try (x_1 - x_2) and (y_1 - y_2) nodes...
        //if x stays the same, then y changes
        if((x_1 - x_2) == 0){
            if((y_1 - 1) >= 0 && (y_1 + 1) < s){
                if((y_1 - y_2) == -2){ grid[x_1][y_1 - 1] = 1; } //west
                if((y_1 - y_2) == 2){ grid[x_1][y_1 + 1] = 1; } //east
            }
        }
        //else if y stays the same, then x changes
        else if((y_1 - y_2) == 0){
            if((x_1 - 1) >= 0 && (x_1 + 1) < s){
                if((x_1 - x_2) == -2){ grid[x_1 - 1][y_1] = 1; } //north
                if((x_1 - x_2) == 2){ grid[x_1 + 1][y_1] = 1; } //south
            }
        }

        //Then try (x_2 - x_1) and (y_2 - y_1) nodes...
        //if x stays the same, then y changes
        if((x_2 - x_1) == 0){
            if((y_2 - 1) >= 0 && (y_2 + 1) < s){
                if((y_2 - y_1) == -2){ grid[x_2][y_2 - 1] = 1; } //west
                if((y_2 - y_1) == 2){ grid[x_2][y_2 + 1] = 1; } //east
            }
        }
        //else if y stays the same, then x changes
        else if((y_2 - y_2) == 0){
            if((x_2 - 1) >= 0 && (x_2 + 1) < s){
                if((x_2 - x_1) == -2){ grid[x_2 - 1][y_2] = 1; } //north
                if((x_2 - x_1) == 2){ grid[x_2 + 1][y_2] = 1; } //south
            }
        }

    }

    printf("\n\nMaze Grid:\n\n");
    for(int i = 0; i < s; ++i){
        for(int j = 0; j < s; ++j){ printf("%d ", grid[i][j]); }
        printf("\n");
    }
    printf("\nNote: 0,0 is the left-most top-most corner\n");
    printf("and 8,8 is the right-most bottom-most corner\n");

    //********************** NOTE **********************
    //Let user pick source and destination coordinates
    //since each graph will be different, we don't know
    //where the paths will lead.

    int source_x, source_y;
    int dest_x, dest_y;

    printf("\n\nPlease enter coordinates for...\n");
    printf("Example: 4,2\n");
    printf("Source: ");
    scanf("%d,%d", &source_x, &source_y);
    printf("Destination: ");
    scanf("%d,%d", &dest_x, &dest_y);

    const int sx = source_x;
    const int sy = source_y;
    const int dx = dest_x;
    const int dy = dest_y;

    pair<int, int> source = make_pair(sx, sy); //Source is the left-most bottom-most corner
	pair<int, int> dest = make_pair(dx, dy); //Destination is the left-most top-most corner
	Astar newAstar;
	newAstar.generatePath(source, dest, grid);

	return 0;
}
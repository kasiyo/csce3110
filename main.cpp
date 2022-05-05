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
    //Let user pick size of grid by choosing the length/width
    //of Minimum Spanning Tree (MST) nodes
    //aka how many nodes in each row/column
    printf("\nWelcome, please enter maze size: ");
    int n; scanf("%d", &n);
    //Length and width will be the same since it will be a square maze/graph

    int SIZE = pow(n, 2); //size of prim's mst
	Prim prim(n, SIZE); //Prim algorithm object
    vector<int> mst; //minimum spanning tree
	mst = prim.Print(); //Prints new graph/maze's list of open paths/edges

    int s = 2*n - 1; //grid size
    //Dynamically allocate grid[][] 2D array in order to pass through parameters
    //Alternatively you can use 2D vectors instead of doing this
    int** grid = new int*[s];
    for(int i = 0; i < s; i++){
        grid[i] = new int[s];
        for(int j = 0; j < s; j++){
            grid[i][j] = 0;
        }
    }
    //Maze will be filled with 1's (open paths) and 0's (closed paths)
    //Nodes are separated by a 1 or 0, where a 1 means it's connected 
    //to another node and part of an open path

    
    //Filling grid with open paths
    int x_1, y_1, x_2, y_2;
    for(int i = 1; i < SIZE; ++i){
        //First node
        x_1 = (i % n) * 2;
        y_1 = (i / n) * 2;
        grid[x_1][y_1] = 1;

        //Second node
        x_2 = (mst[i] % n) * 2;
        y_2 = (mst[i] / n) * 2;
        grid[x_2][y_2] = 1;

        /*
        Finding the middle open "path"/edge
        connects the two nodes together...
        Note: x is actually the y axis and y is
        the x axis if you're imagining the grid
        as a cartesean plane. Remember that each
        row will output its column values.

        First try (x_1 - x_2) and (y_1 - y_2) nodes...
        if x stays the same, then y changes
        */
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
    printf("\nMaze Grid Note: 0,0 is the left-most top-most corner and\n");
    printf("(2*n - 2),(2*n - 2) is the right-most bottom-most corner\n");


    //Let user pick source and destination coordinates
    //since each graph will be different, we don't know
    //where the paths will lead
    int source_x, source_y;
    int dest_x, dest_y;

    printf("\n\nPlease enter coordinates for...\n");
    printf("Example: 0,0\n");
    printf("Source: ");
    scanf("%d,%d", &source_x, &source_y);
    printf("Destination: ");
    scanf("%d,%d", &dest_x, &dest_y);

    //Source is the left-most bottom-most corner
    pair<int, int> source = make_pair(source_x, source_y);
    //Destination is the left-most top-most corner
	pair<int, int> dest = make_pair(dest_x, dest_y);
	Astar astar(s);
	astar.generatePath(source, dest, grid);

	return 0;
}
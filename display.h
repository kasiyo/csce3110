#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <utility>
#include <list>
using namespace std;

typedef struct cell {
    pair<int, int> x_y;
    int index;
    int num_id;
    char cell_id;
    vector<pair<int, int>> adj_coords;
    bool north_wall, south_wall, east_wall, west_wall;
    bool north_neighbor, south_neighbor, east_neighbor, west_neighbor;
    struct cell* north;
    struct cell* south;
    struct cell* east;
    struct cell* west;
} cell;

class display {
    public:
        display();
        ~display();
        display(int n, int** newGridMap, int size);    //parameterized constructor
        
        void mapCells();
        void map_adjacent_cells(cell* cell_to_map);
        int get_coord_index(int x_val, int y_val);
        void add_frontier(pair<int, int> coord, vector<pair<int, int>> vec);

        void printCells();
        void createMap();
        void displayMap();
        void test();
    private:
        int** blueprint;
        char** gridMap;
        
        vector<char> top_bot_border;
        vector <vector <char>> mapLayers;
        vector <vector <char>> content_layers;

        int userInput;
        int sq_side_length, num_columns, num_rows;
        int num_cells_per_row, totalCells;
        int num_vertices, num_edges, last_index;

        vector<int> cell_row_indices;
        vector<cell*> cellMap;
        
        vector <pair<int, int> > coord_pairs;
        vector <pair<int, int> > frontier;
        vector <pair <cell*, pair<int, int> > > cell_coord_pairs;
};

#endif
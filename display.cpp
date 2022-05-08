#include "display.h"

cell* newCell(int index_location, int xVal, int yVal, int id) {
    struct cell* temp = (struct cell*)malloc(sizeof(struct cell));
    temp->index = index_location;
    temp->x_y = make_pair(xVal, yVal);
    
    if (temp->x_y.first == 0) {temp->north_wall = true;}
    temp->num_id = id;

    if (temp->num_id == 0) {temp->cell_id = '|';}
    else if (temp->num_id == 1) {temp->cell_id = ' ';}
    temp->north = temp->south = temp->west = temp->east = nullptr;
    temp->north_neighbor = temp->south_neighbor = temp->east_neighbor = temp->west_neighbor = false;
    temp->north_wall = temp->south_wall = temp->east_wall = temp->west_wall = false;
    
    return temp;
}

display::display() {
    blueprint = nullptr;
    gridMap = nullptr;
    sq_side_length = -1;
    num_columns = num_rows = -1;
    num_vertices = num_edges = -1;
}

display::~display() {
    for (int i = 0; i < cellMap.size(); ++i) {
        free(cellMap[i]);
    }
}

display::display(int n, int** newGridMap, int size) {
    this->userInput = n;
    this->blueprint = newGridMap;

    sq_side_length = size;
    
    num_columns = (userInput * 3) + ((2 * userInput) - 2);

    num_rows = (userInput * 2) + sq_side_length;

    num_vertices = num_columns + num_rows;
    num_edges = num_vertices + userInput;
    num_cells_per_row = (2 * n) - 2;
}

//maps the cells into a vector w coordinate pairs
void display::mapCells() {
    cell* cellPtr;
    pair<int, int> coord_pair;
    pair<cell*, pair<int, int>> cell_coords;
    int index = 0;
    for (int i = 0; i < sq_side_length; ++i) {
        for (int j = 0; j < sq_side_length; ++j) {
            coord_pair = make_pair(i, j);
            cellPtr = newCell(index, i, j, blueprint[i][j]);

            cellMap.push_back(cellPtr); 
            coord_pairs.push_back(coord_pair);

            cell_coords = make_pair(cellPtr, coord_pair);
            cell_coord_pairs.push_back(cell_coords);
            ++index;
        }
    }
    totalCells = index;
    
    for (auto it : cell_coord_pairs) {map_adjacent_cells(it.first);}
}

//returns the index of a cell by it's coordinates
int display::get_coord_index(int x_val, int y_val) {
    for (int i = 0; i < coord_pairs.size(); ++i) {
        if (coord_pairs[i].first == x_val && coord_pairs[i].second == y_val) {
            return i;
        }
    }
    return -1;
}

//maps the adjacent cells and determines if there are surrounding walls
void display::map_adjacent_cells(cell* cell_to_map) {
    int maze_cols = sq_side_length - 1;  //num of cols and rows to compare coords to
    int maze_rows = sq_side_length;
    
    cell* explorer;  //initialize it to the first cell

    for (auto it : cell_coord_pairs) {
        explorer = it.first;        //first is cell*
        int x = it.second.first;    //second.first is x-coord
        int y = it.second.second;   //second.second is y-coord

        if (x >= 0 && x < maze_cols && y - 1 >= 0 && y - 1 < maze_rows) {   //north
            int north_index = get_coord_index(x, y - 1);

            explorer->north_neighbor = true;
            explorer->north = (struct cell*)malloc(sizeof(struct cell));
            explorer->north = cellMap[north_index];

            if (blueprint[x][y - 1] == 0) {explorer->north_wall = true;}
        }
        
        if (x >= 0 && x < maze_cols && y + 1 >= 0 && y + 1 < maze_rows) {   //south
            int south_index = get_coord_index(x, y + 1);

            explorer->south_neighbor = true;
            explorer->south = (struct cell*)malloc(sizeof(struct cell));
            explorer->south = cellMap[south_index];

            if (blueprint[x][y + 1] == 0) {explorer->south_wall = true;}
        }
        
        if (x - 1 >= 0 && x < maze_cols && y >= 0 && y < maze_rows) {   //east
            int east_index = get_coord_index(x - 1, y);

            explorer->east_neighbor = true;
            explorer->east = (struct cell*)malloc(sizeof(struct cell));
            explorer->east = cellMap[east_index];

            if (blueprint[x - 1][y] == 0) {explorer->east_wall = true;}
        }

        if (x + 1 >= 0 && x < maze_cols && y >= 0 && y < maze_rows) {   //west
            int west_index = get_coord_index(x + 1, y);

            explorer->west_neighbor = true;
            explorer->west = (struct cell*)malloc(sizeof(struct cell));
            explorer->west = cellMap[west_index];

            if (blueprint[x + 1][y] == 0) {explorer->west_wall = true;}
        }
    }
}

//print cell values
void display::printCells() {
    int currCell = 0;
    for (auto it : cellMap) {
        cout << "[" << it->x_y.first << "][" << it->x_y.second << "]: " << it->cell_id;
        if (currCell < num_cells_per_row) {
            cout << string(3, ' ');
            ++currCell;
        }
        else {
            cout << "\n";
            currCell = 0;
        }
    }
    cout << "\n";

    currCell = 0;
    for (auto it : cell_coord_pairs) {
        cout << "cell[" << it.first->index << "] @ " << it.second.first << ", " << it.second.second;
        if (currCell < num_cells_per_row) {
            cout << string(3, ' ');
            ++currCell;
        }
        else {
            cout << "\n";
            currCell = 0;
        }
    }
    cout << "\n\n";
}

//create layers for each line of the map
void display::createMap() {
    vector<char> border;
    vector<char> row_content;
    vector<char> row_divider;
    vector <vector <char>> charMap;
    int curr_row = 0, curr_col = 0, curr_index = 0;

    for (int i = 0; i < 5; ++i) {border.push_back('-');}

    for (int i = 0; i < num_columns; ++i) {
        switch(i % 4) {
            case 0: {border.push_back('+'); break;}
            case 1: case 2: case 3: {border.push_back(' '); break;}
        }
    }

    for (int i = 0; i < border.size(); ++i) {
        if (border[i] == ' ') {top_bot_border.push_back('-');}
        else {top_bot_border.push_back(border[i]);}
    }

    int numCells = (userInput * 2) - 1;
    int row_index = 0, layer_index = 0;

    int total_cell_rows = userInput * sq_side_length;
    charMap.push_back(top_bot_border);

    //start map layer with northern border of maze
    mapLayers.push_back(top_bot_border);
    ++layer_index;

    for (curr_row = 0; curr_row < sq_side_length; ++curr_row) {
        row_content.push_back(' ');             //creates y-axis column
        row_content.push_back(' ');
        row_content.push_back(curr_row + '0');
        row_content.push_back(' ');
        row_content.push_back(' ');
        
        row_content.push_back('|');
        
        for (int fill = 0; fill < 5; fill++) {row_divider.push_back('-');}
        
        //build east/west walls
        for (curr_col = 0; curr_col < sq_side_length; ++curr_col) {
            row_content.push_back(' ');
            if (blueprint[curr_row][curr_col] == 0) {row_content.push_back('|');}
            if (blueprint[curr_row][curr_col] == 1) {row_content.push_back(' ');}
        }
        
        //build north/south walls
        for (int i = 0; i < sq_side_length; ++i) {
            if (i == 0) {row_divider.push_back('+');}
            //check if north/south wall can be built
            switch(i % 2) {
                case 0: {
                    for (int fill = 0; fill < 3; fill++) {
                        if (blueprint[curr_row][i] == 0) {row_divider.push_back('-');}
                        if (blueprint[curr_row][i] == 1) {row_divider.push_back(' ');}
                    }
                    break;
                }
                //north/south wall cannot be built, enclose w '+'
                case 1: {row_divider.push_back('+'); break;}
            }
        }
        row_divider.push_back('+');     //end of current row
        row_content.push_back(' ');
        content_layers.push_back(row_content);
        
        charMap.push_back(row_content);
        charMap.push_back(border);

        mapLayers.push_back(row_content);
        cell_row_indices.push_back(layer_index);
        ++layer_index;
        
        //insert row divider after each row of content
        if (curr_row < (sq_side_length - 1)) {mapLayers.push_back(row_divider);}
        if (curr_row == sq_side_length - 1) {mapLayers.push_back(top_bot_border);}
        row_divider.clear();
        cell_row_indices.push_back(layer_index);
        ++layer_index;
        row_content.clear();   
    }
}

//prints out the map
void display::displayMap() {
    cout << "\n\n\nMaze Visualization:\n\n";
    cout << " \\ X |" << "\n";
    cout << "Y \\  | ";
    
    for (int i = 0; i < sq_side_length; ++i) {cout << i << " ";}

    for (int i = 0; i < mapLayers.size(); i++) {
        if (i == 0) {cout << "\n";}
        for (int j = 0; j < mapLayers[i].size(); ++j) {
            cout << mapLayers[i][j];
        }
        if (i % 2 == 1) {cout << "|";}  //builds eastern border of maze
        cout << "\n";
    }
    cout << "\n\n";
}

//test environment
void display::test() {

    cout << "\n\n";
    
    mapCells();

    createMap();
    displayMap();

    cout << "\n\n";
}
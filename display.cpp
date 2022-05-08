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
    numCorners = -1;
    sq_side_length = -1;
    num_columns = num_rows = -1;
    num_vertices = num_edges = -1;
}

display::~display() {
    free(blueprint);
    free(gridMap);
}

display::display(int n, int** newGridMap, int size) {
    this->userInput = n;
    this->blueprint = newGridMap;

    sq_side_length = size;
    this->numCorners = size + 1;
    
    num_columns = (userInput * 3) + ((2 * userInput) - 2);

    num_rows = (userInput * 2) + sq_side_length;

    num_vertices = num_columns + num_rows;
    num_edges = num_vertices + userInput;
    num_cells_per_row = (2 * n) - 2;
}

void display::add_frontier(pair<int, int> coord, vector<pair<int, int>> vec) {
    if (coord.first >= 0 && coord.second >= 0 && coord.second < sq_side_length && coord.first < num_cells_per_row && blueprint[coord.second][coord.first] == 0) {
        vec.push_back(coord);
    }
}

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

int display::get_coord_index(int x_val, int y_val) {
    for (int i = 0; i < coord_pairs.size(); ++i) {
        if (coord_pairs[i].first == x_val && coord_pairs[i].second == y_val) {
            return i;
        }
    }
    return -1;
}

void display::map_adjacent_cells(cell* cell_to_map) {
    vector<cell*> unvisited;
    vector<cell*> visited;
    vector<int> u;
    vector<int> v;
    vector<pair<int, int>> personal_frontier;
    pair<int, int> f_coord;

    int maze_cols = sq_side_length - 1;  //num of cols and rows to compare coords to
    int maze_rows = sq_side_length;

    for (int i = 1; i < cellMap.size(); ++i) {unvisited.push_back(cellMap[i]);}
    for (int i = 0; i < totalCells; ++i) {u.push_back(i);}
    
    cell* explorer;  //initialize it to the first cell

    for (auto it : cell_coord_pairs) {
        explorer = it.first;        //first is cell*
        int x = it.second.first;    //second.first is x-coord
        int y = it.second.second;   //second.second is y-coord

        if (x >= 0 && x < maze_cols && y - 1 >= 0 && y - 1 < maze_rows) {   //north
            int north_index = get_coord_index(x, y - 1);
            f_coord = make_pair(x, y - 1);

            explorer->north_neighbor = true;
            explorer->north = cellMap[north_index];
            explorer->adj_cell_indices.push_back(north_index);

            if (blueprint[x][y - 1] == 0) {explorer->north_wall = true;}


            add_frontier(f_coord, frontier);
            personal_frontier.push_back(f_coord);
            
        }
        
        if (x >= 0 && x < maze_cols && y + 1 >= 0 && y + 1 < maze_rows) {   //south
            int south_index = get_coord_index(x, y + 1);
            f_coord = make_pair(x, y + 1);

            explorer->adj_cell_indices.push_back(south_index);
            explorer->south_neighbor = true;
            explorer->south = cellMap[south_index];

            if (blueprint[x][y + 1] == 0) {explorer->south_wall = true;}
            
            add_frontier(f_coord, frontier);
            
            personal_frontier.push_back(f_coord);
            
        }
        
        if (x - 1 >= 0 && x < maze_cols && y >= 0 && y < maze_rows) {   //east
            int east_index = get_coord_index(x - 1, y);
            f_coord = make_pair(x - 1, y);

            for (auto it : explorer->adj_cell_indices) {
                if (it == east_index) {}
            }
            explorer->adj_cell_indices.push_back(east_index);
            explorer->east_neighbor = true;
            explorer->east = cellMap[east_index];

            if (blueprint[x - 1][y] == 0) {explorer->east_wall = true;}

            personal_frontier.push_back(f_coord);

            frontier.push_back(f_coord);
        }

        if (x + 1 >= 0 && x < maze_cols && y >= 0 && y < maze_rows) {   //west
            int west_index = get_coord_index(x + 1, y);
            f_coord = make_pair(x + 1, y);
            
            explorer->adj_cell_indices.push_back(west_index);
            explorer->west_neighbor = true;
            explorer->west = cellMap[west_index];

            if (blueprint[x + 1][y] == 0) {explorer->west_wall = true;}

            personal_frontier.push_back(f_coord);

            frontier.push_back(f_coord);
        }

    }
}

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

//create map version 2
void display::makeMap() {
    vector<char> border;
    vector<char> row_content;
    vector<char> row_divider;
    vector <vector <char>> charMap;
    vector<char> row_layer;
    vector<char> cell_layer;

    int curr_cell_in_row = 0;
    int curr_row = 0, curr_col = 0, curr_index = 0;
    int cell_col = 0, cell_row = 0, next_cell_col = cell_col + 2;

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
    int row_index = 0, currCell = 0, layer_index = 0;
    pair<int, int> coord_pair;
    pair<int, int> coord_to_compare;

    int total_cell_rows = userInput * sq_side_length;
    charMap.push_back(top_bot_border);

    //start map layer with northern border of maze
    mapLayers.push_back(top_bot_border);
    ++layer_index;

    for (curr_row = 0; curr_row < sq_side_length; ++curr_row) {
        int currCell = 0;
        coord_pair = cell_coord_pairs[currCell].second;
        coord_to_compare = cell_coord_pairs[currCell + 1].second;

        //coord_cell_id = cell_coord_pairs[currCell].first
        row_content.push_back(' ');
        row_content.push_back(' ');
        row_content.push_back(curr_row + '0');
        
        row_content.push_back(' ');
        row_content.push_back(' ');

        
        row_content.push_back('|');
        for (curr_col = 0; curr_col < sq_side_length; ++curr_col) {
            coord_to_compare = make_pair(curr_row, curr_col);
            cell* temp = cellMap[get_coord_index(curr_row, curr_col)];
            
            row_content.push_back(' ');
            if (blueprint[curr_row][curr_col] == 0) {row_content.push_back('|');}
            if (blueprint[curr_row][curr_col] == 1) {row_content.push_back(' ');}
        }
        row_content.push_back(' ');
        content_layers.push_back(row_content);

        
        charMap.push_back(row_content);
        charMap.push_back(border);

        mapLayers.push_back(row_content);
        cell_row_indices.push_back(layer_index);
        ++layer_index;
        if (curr_row < (sq_side_length - 1)) {mapLayers.push_back(border);}
        if (curr_row == sq_side_length - 1) {mapLayers.push_back(top_bot_border);}
        
        cell_row_indices.push_back(layer_index);
        ++layer_index;
        row_content.clear();    //end of current row
    }
    
    
    
    int x = 0, y = 0;


    curr_row = 0, curr_col = 0;
    
    for (int i = 0; i < dividing_layers.size(); ++i) {//build north/south walls
        cout << "dividing_layers[" << i << "]: ";
        for (int j = 0; j < dividing_layers[i].size(); ++j) {
            cout << dividing_layers[i][j];
        }
        cout << "\n";
    }
    int curr_cell = 0;
    /*for (int i = 0; i < content_layers.size(); ++i) {//build east/west walls
        int last_col = content_layers[i].size();

        
        for (int j = 0; j < sq_side_length; ++j) {
            
            if (blueprint[i][j] == 0) {content_layers[i].push_back('|');}
            if (blueprint[i][j] == 1) {content_layers[i].push_back(' ');}
            content_layers[i].push_back(' ');
        }
        for (int j = 0; j < content_layers[i].size(); ++j) {
            cout << content_layers[i][j];
        }
        cout << "\n";

    }*/

    curr_cell = 0;
    /*for (int i = 0; i < num_rows; ++i) {
        if (i % 2 == 0) {
            for (int j = 0; j < dividing_layers[i].size(); ++j) {
                cout << dividing_layers[i][j];
            }
        }
        if (i % 2 == 1) {
            for (int j = 0; j < content_layers[i].size(); ++i) {
                cout << content_layers[i][j];
            }
        }
        cout << "\n";
    }*/

}

void display::createMap() {
    //grid 2
    char* horizontal_wall = new char[(userInput*2)+((userInput*2) - 1)];
    vector<char> row_divider;
    vector<char> cell_content;
    vector <vector <char>> baseLayer;
    char row;
    gridMap = new char*[num_rows];

    int numNodes = (userInput * 2) - 1;
    cout << "numNodes = " << numNodes << "\n";
    int col_size = 3;

    int x = 0, y = 0;
    bool hasWall = false;

    for (int i = 0; i < num_rows; ++i) {
        gridMap[i] = new char[num_columns];
        if (i % 2 == 0) {
            for (int j = 0; j < num_columns; ++j) {
                gridMap[i][j] = ' ';
                switch(j % 4) {
                    case 0: {
                        gridMap[i][j] = '+';
                        row_divider.push_back('+');
                        break;
                    }
                    case 1: case 2: /*case 3:*/ {
                        gridMap[i][j] = '-';
                        row_divider.push_back(' ');

                        break;
                    }
                }
            }
            row_divider.push_back('\n');
            
            baseLayer.push_back(row_divider);
            row_divider.clear();
        }
        else if (i % 2 == 1) {
            for (int j = 0; j < num_columns; ++j) {
                switch(j % 4) {
                    case 0: {
                        gridMap[i][j] = '|';
                        //cell_content.push_back('|');
                        break;
                    }
                    case 1: {
                        gridMap[i][j] = ' ';
                        cell_content.push_back(' ');
                        break;
                    }
                    case 2: {
                        gridMap[i][j] = '?';
                        if (blueprint[y][x] == 1) {
                            cell_content.push_back(' ');
                            gridMap[i][j] = ' ';
                        }
                        if (blueprint[y][x] == 0) {
                            cell_content.push_back('|');
                            gridMap[i][j] = '|';
                        }
                        ++x;
                        break;
                    }

                }
            }
            x = 0;
            ++y;
            cell_content.push_back('\n');
            baseLayer.push_back(cell_content);
            cell_content.clear();
        }   
    }
    for (int i = 0; i < baseLayer.size(); ++i) {
        for (int j = 0; j < baseLayer[i].size(); ++j) {
            
            cout << baseLayer[i][j];
        }
    }
    cout << "\n\n";
}

void display::displayMap() {
    cout << " \\ X |" << "\n";
    cout << "Y \\  | ";
    
    for (int i = 0; i < sq_side_length; ++i) {cout << i << " ";}

    for (int i = 0; i < mapLayers.size(); i++) {
        if (i == 0) {cout << "\n";}
        for (int j = 0; j < mapLayers[i].size(); ++j) {
            cout << mapLayers[i][j];
        }
        if (i % 2 == 1) {cout << "|";}
        cout << "\n";
    }

    cout << "\n\n";
}

void display::test() {
    //cout << "blueprint = \n";
    //cout << blueprint << "\n";

    cout << "\n\n";
    //createMap();
    mapCells();
    //printCells();
    makeMap();
    displayMap();


    cout << "\n\n";
}
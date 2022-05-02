//A* Algorithm CPP File
#include "astar.h"
using namespace std;

double Astar::getHeuristic(int x, int y, int destX, int destY){
	return (abs(x - destX) + abs(y - destY));
}

bool Astar::nodeBoundCheck(int x, int y){
	//If x and y is in range, then return true
	return ((x >= 0) && (x < MAX_ROW) && (y >= 0) && (y < MAX_COL));
}
bool Astar::nodeBlockCheck(int x, int y, int grid[][MAX_COL]){
	//If node is not blocked, then return true, else false
	if(grid[x][y] == 1) {return true;} else {return false;}
}

bool Astar::destReached(int x, int y, int destX, int destY){
	if((x == destX) && (y == destY)) {return true;} else {return false;}
}

void Astar::tracePath(node details[][MAX_COL], Pair dest){
	int x = dest.first;
	int y = dest.second;

	stack<Pair> Path;

	while(!(details[x][y].parent_i == x && details[x][y].parent_j == y)){
		Path.push(make_pair(x, y));
		int temp_x = details[x][y].parent_i;
		int temp_y = details[x][y].parent_j;
		x = temp_x;
		y = temp_y;
	}

	Path.push(make_pair(x, y));
	while(!Path.empty()){
		pair<int, int> p = Path.top();
		Path.pop();
		printf("-> (%d, %d) ", p.first, p.second);
	}

	return;
}

void Astar::generatePath(pair<int, int> source, pair<int, int> dest, int grid[][MAX_COL]){

	//Check for errors
	if(grid){
		if(!nodeBoundCheck(source.first, source.second))
		{printf("A* ERROR: Source Node out of range. x: %d, y: %d\n",source.first, source.second);}
		if(!nodeBoundCheck(dest.first, dest.second))
		{printf("A* ERROR: Destination Node out of range. x: %d, y: %d\n", dest.first, dest.second);}
		if(!nodeBlockCheck(source.first, source.second, grid))
		{printf("A* ERROR: Source Node in the wall. x: %d, y: %d\n", source.first, source.second);}
		if(!nodeBlockCheck(dest.first, dest.second, grid))
		{printf("A* ERROR: Destination Node in the wall. x: %d, y: %d\n", dest.first, dest.second);}

		//Creates a table of booleans
		//false = node not included yet
		bool closedList[MAX_ROW][MAX_COL];
		memset(closedList, false, sizeof(closedList));

		//Create a table of nodes -> node details
		node details[MAX_ROW][MAX_COL];
		int i, j;
		for(i = 0; i < MAX_ROW; i++){
			for(j = 0; j < MAX_COL; j++){
				node temp = node();
				details[i][j] = temp;
			}
		}

		//Sets parameters of starting node
		i = source.first, j = source.second;
		details[i][j].f = 0.0f;
		details[i][j].g = 0.0f;
		details[i][j].h = 0.0f;
		details[i][j].parent_i = i;
		details[i][j].parent_j = j;

		
		//Create priority queue
		set<pPair> openList; //info: <f, <i, j>>

		openList.insert(make_pair(0.0f, make_pair(i, j)));
		//Puts the starting cell on the open list and sets its 'f' as 0

		bool foundDest = false; //destination not found yet

		//Logic loop
		while(!openList.empty()){

			//Pop first
			pPair p = *openList.begin();
			openList.erase(openList.begin());

			//Mark in closed list
			i = p.second.first;
			j = p.second.second;
			closedList[i][j] = true;
			
			/*
			Generating all the 4 successors of this node...
 
					N  
					| 
				W---- Node ----E
			 		|
					S
			
			Cell-->Popped Cell (i, j)
         	N -->  North       (i-1, j)
         	S -->  South       (i+1, j)
         	E -->  East        (i, j+1)
         	W -->  West        (i, j-1)
			*/

			double ftemp, gtemp, htemp; //storing the f, g, and h of the successors

			//------------------------ North (i - 1, j) ------------------------
			if(nodeBoundCheck(i - 1, j)){

				//Check if destination is reached
				if(destReached(i - 1, j, dest.first, dest.second)){
					//set destination node's parent
					details[i - 1][j].parent_i = i;
					details[i - 1][j].parent_j = j;

					printf("Destination Node found!\n");

					foundDest = true;
					tracePath(details, dest);
					return;
				}
				//If the north successor is neither visited (on closedList) nor blocked, then...
				else if((closedList[i - 1][j] == false) && (nodeBlockCheck(i - 1, j, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0; //+ 1.414 instead if it was a diagonal
					htemp = getHeuristic(i - 1, j, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//If not on openList OR if cost is lower,
					//then add it to openList and update node details
					if((details[i - 1][j].f  == FLOAT_MAX) || (details[i - 1][j].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i - 1, j)));
						
						//Update node details
						details[i - 1][j].f = ftemp;
						details[i - 1][j].g = gtemp;
						details[i - 1][j].h = htemp;
						details[i - 1][j].parent_i = i;
						details[i - 1][j].parent_j = j;
					}
				}
			}

			//------------------------ South (i + 1, j) ------------------------
			if(nodeBoundCheck(i + 1, j)){

				//Check if destination is reached
				if(destReached(i + 1, j, dest.first, dest.second)){
					//set destination node's parent
					details[i + 1][j].parent_i = i;
					details[i + 1][j].parent_j = j;
					foundDest = true;

					printf("Destination Node found!\n");

					tracePath(details, dest);
					return;
				}
				//If the south successor is neither visited (on closedList) nor blocked, then...
				else if((closedList[i + 1][j] == false) && (nodeBlockCheck(i + 1, j, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i + 1, j, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//If not on openList OR if cost is lower,
					//then add it to openList and update node details
					if((details[i + 1][j].f  == FLOAT_MAX) || (details[i + 1][j].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i + 1, j)));

						//Update node details
						details[i + 1][j].f = ftemp;
						details[i + 1][j].g = gtemp;
						details[i + 1][j].h = htemp;
						details[i + 1][j].parent_i = i;
						details[i + 1][j].parent_j = j;
					}
				}
			}

			
			//------------------------ East (i, j + 1) -------------------------
			if(nodeBoundCheck(i, j + 1)){

				//Check if destination is reached
				if(destReached(i, j  + 1, dest.first, dest.second)){
					//set destination node's parent
					details[i][j + 1].parent_i = i;
					details[i][j + 1].parent_j = j;
					foundDest = true;

					printf("Destination Node found!\n");

					tracePath(details, dest);
					return;
				}
				//If the east successor is neither visited (on closedList) nor blocked, then...
				else if((closedList[i][j + 1] == false) && (nodeBlockCheck(i, j + 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i, j + 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//If not on openList OR if cost is lower,
					//then add it to openList and update node details
					if((details[i][j + 1].f  == FLOAT_MAX) || (details[i][j + 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i, j + 1)));

						//Update node details
						details[i][j + 1].f = ftemp;
						details[i][j + 1].g = gtemp;
						details[i][j + 1].h = htemp;
						details[i][j + 1].parent_i = i;
						details[i][j + 1].parent_j = j;
					}
				}
			}


			//------------------------ West (i, j - 1) -------------------------
			if(nodeBoundCheck(i, j - 1)){

				//Check if destination is reached
				if(destReached(i, j  - 1, dest.first, dest.second)){
					//set destination node's parent
					details[i][j - 1].parent_i = i;
					details[i][j - 1].parent_j = j;
					foundDest = true;

					printf("Destination Node found!\n");

					tracePath(details, dest);
					return;
				}
				//If the west successor is neither visited (on closedList) nor blocked, then...
				else if((closedList[i][j - 1] == false) && (nodeBlockCheck(i, j - 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i, j - 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//If not on openList OR if cost is lower,
					//then add it to openList and update node details
					if((details[i][j - 1].f  == FLOAT_MAX) || (details[i][j - 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i, j - 1)));

						//Update node details
						details[i][j - 1].f = ftemp;
						details[i][j - 1].g = gtemp;
						details[i][j - 1].h = htemp;
						details[i][j - 1].parent_i = i;
						details[i][j - 1].parent_j = j;
					}
				}
			}
		}

		if(foundDest == false){printf("A* ERROR: Path not found.\n");}

	} else {printf("A* ERROR: Not initalized\n");}

	return;

}


#include "astar.h"
using namespace std;

double Astar::getEuclideanDistance(int x, int y){return sqrt((x * x)+(y * y));}
double Astar::getHeuristic(int x, int y, int destX, int destY){
	return getEuclideanDistance((x - destX), (y - destY));
}

bool Astar::nodeBoundCheck(int x, int y){
	return (!(x >= 0 && x <= MAX_ROW && y >= 0 && y <= MAX_COL));
}
bool Astar::nodeBlockCheck(int x, int y, int grid[][MAX_COL]){
	if(grid[x][y] == 1){
		return true;
	} else {
		return false;
	}
}

bool Astar::destReached(int x, int y, int destX, int destY){
	printf("x: %d, y: %d, dest_x: %d, dest_y: %d\n", x, y, destX, destY);
	if((x == destX) && (y == destY)){
		return true;
	}else{
		return false;
	}
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
		if(nodeBoundCheck(source.first, source.second)){printf("A* ERROR: Source Node out of range. x: %d, y: %d\n", source.first, source.second);}
		if(nodeBoundCheck(dest.first, dest.second)){printf("A* ERROR: Destination Node out of range. x: %d, y: %d\n", dest.first, dest.second);}
		if(!nodeBlockCheck(source.first, source.second, grid)){printf("A* ERROR: Source Node in the wall. x: %d, y: %d\n", source.first, source.second);}
		if(!nodeBlockCheck(dest.first, dest.second, grid)){printf("A* ERROR: Destination Node in the wall. x: %d, y: %d\n", dest.first, dest.second);}

		//Creates a table of booleans
		bool closedList[MAX_ROW][MAX_COL];
		memset(closedList, false, sizeof(closedList));

		//Create a table of nodes
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
		set<pPair> openList;
		openList.insert(make_pair(0.0f, make_pair(i, j)));
		bool foundDest = false;

		//Logic loop
		while(!openList.empty()){

			printf("x: %d , y: %d\n", i, j);

			//Pop first
			pPair p = *openList.begin();
			openList.erase(openList.begin());

			//Mark in closed list
			i = p.second.first;
			j = p.second.second;
			closedList[i][j] = true;

			double ftemp, gtemp, htemp;

			//North (i - 1, j)
			if(nodeBoundCheck(i - 1, j)){

				//Check if destination is reached
				if(destReached(i - 1, j, dest.first, dest.second)){
					details[i - 1][j].parent_i = i;
					details[i - 1][j].parent_j = j;

					//REMOVE LATER
					printf("Node found!\n");

					foundDest = true;
					tracePath(details, dest);
					return;
				}
				//Check if north node is not visited and not blocked
				else if((closedList[i - 1][j] == false) && (nodeBlockCheck(i - 1, j, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i - 1, j, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i - 1][j].f  == FLOAT_MAX) || (details[i - 1][j].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i - 1, j)));
						
						details[i - 1][j].f = ftemp;
						details[i - 1][j].g = gtemp;
						details[i - 1][j].h = htemp;
						details[i - 1][j].parent_i = i;
						details[i - 1][j].parent_j = j;
					}
				}
			}

			//South (i + 1, j)
			if(nodeBoundCheck(i + 1, j)){

				//Check if destination is reached
				if(destReached(i + 1, j, dest.first, dest.second)){
					details[i + 1][j].parent_i = i;
					details[i + 1][j].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if south node is not visited and not blocked
				else if((closedList[i + 1][j] == false) && (nodeBlockCheck(i + 1, j, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i + 1, j, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i + 1][j].f  == FLOAT_MAX) || (details[i + 1][j].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i + 1, j)));

						details[i + 1][j].f = ftemp;
						details[i + 1][j].g = gtemp;
						details[i + 1][j].h = htemp;
						details[i + 1][j].parent_i = i;
						details[i + 1][j].parent_j = j;
					}
				}
			}

			
			//East (i, j + 1)
			if(nodeBoundCheck(i, j + 1)){

				//Check if destination is reached
				if(destReached(i, j  + 1, dest.first, dest.second)){
					details[i][j + 1].parent_i = i;
					details[i][j + 1].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if east node is not visited and not blocked
				else if((closedList[i][j + 1] == false) && (nodeBlockCheck(i, j + 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i, j + 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i][j + 1].f  == FLOAT_MAX) || (details[i][j + 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i, j + 1)));
						
						details[i][j + 1].f = ftemp;
						details[i][j + 1].g = gtemp;
						details[i][j + 1].h = htemp;
						details[i][j + 1].parent_i = i;
						details[i][j + 1].parent_j = j;
					}
				}
			}


			//West (i, j - 1)
			if(nodeBoundCheck(i, j - 1)){

				//Check if destination is reached
				if(destReached(i, j  - 1, dest.first, dest.second)){
					details[i][j - 1].parent_i = i;
					details[i][j - 1].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if west node is not visited and not blocked
				else if((closedList[i][j - 1] == false) && (nodeBlockCheck(i, j - 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.0;
					htemp = getHeuristic(i, j - 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i][j - 1].f  == FLOAT_MAX) || (details[i][j - 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i, j - 1)));
						
						details[i][j - 1].f = ftemp;
						details[i][j - 1].g = gtemp;
						details[i][j - 1].h = htemp;
						details[i][j - 1].parent_i = i;
						details[i][j - 1].parent_j = j;
					}
				}
			}

			
			//North-East (i - 1, j + 1)
			if(nodeBoundCheck(i - 1, j + 1)){

				//Check if destination is reached
				if(destReached(i - 1, j  + 1, dest.first, dest.second)){
					details[i - 1][j + 1].parent_i = i;
					details[i - 1][j + 1].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if north-east node is not visited and not blocked
				else if((closedList[i - 1][j + 1] == false) && (nodeBlockCheck(i - 1, j + 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.414;
					htemp = getHeuristic(i - 1, j + 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i - 1][j + 1].f  == FLOAT_MAX) || (details[i - 1][j + 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i - 1, j + 1)));
						
						details[i - 1][j + 1].f = ftemp;
						details[i - 1][j + 1].g = gtemp;
						details[i - 1][j + 1].h = htemp;
						details[i - 1][j + 1].parent_i = i;
						details[i - 1][j + 1].parent_j = j;
					}
				}
			}

			
			//North-West (i - 1, j - 1)
			if(nodeBoundCheck(i - 1, j - 1)){

				//Check if destination is reached
				if(destReached(i - 1, j  - 1, dest.first, dest.second)){
					details[i - 1][j - 1].parent_i = i;
					details[i - 1][j - 1].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if north-west node is not visited and not blocked
				else if((closedList[i - 1][j - 1] == false) && (nodeBlockCheck(i - 1, j - 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.414;
					htemp = getHeuristic(i - 1, j - 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i - 1][j - 1].f  == FLOAT_MAX) || (details[i - 1][j - 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i - 1, j - 1)));
						
						details[i - 1][j - 1].f = ftemp;
						details[i - 1][j - 1].g = gtemp;
						details[i - 1][j - 1].h = htemp;
						details[i - 1][j - 1].parent_i = i;
						details[i - 1][j - 1].parent_j = j;
					}
				}
			}


			//South-East (i + 1, j + 1)
			if(nodeBoundCheck(i + 1, j + 1)){

				//Check if destination is reached
				if(destReached(i + 1, j  + 1, dest.first, dest.second)){
					details[i + 1][j + 1].parent_i = i;
					details[i + 1][j + 1].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if south-east node is not visited and not blocked
				else if((closedList[i + 1][j + 1] == false) && (nodeBlockCheck(i + 1, j + 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.414;
					htemp = getHeuristic(i + 1, j + 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i + 1][j + 1].f  == FLOAT_MAX) || (details[i + 1][j + 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i + 1, j + 1)));
						
						details[i + 1][j + 1].f = ftemp;
						details[i + 1][j + 1].g = gtemp;
						details[i + 1][j + 1].h = htemp;
						details[i + 1][j + 1].parent_i = i;
						details[i + 1][j + 1].parent_j = j;
					}
				}
			}
			
			//South-West (i + 1, j - 1)
			if(nodeBoundCheck(i + 1, j - 1)){

				//Check if destination is reached
				if(destReached(i + 1, j  - 1, dest.first, dest.second)){
					details[i + 1][j - 1].parent_i = i;
					details[i + 1][j - 1].parent_j = j;
					foundDest = true;

					//REMOVE LATER
					printf("Node found!\n");

					tracePath(details, dest);
					return;
				}
				//Check if south-west node is not visited and not blocked
				else if((closedList[i + 1][j - 1] == false) && (nodeBlockCheck(i + 1, j - 1, grid) == true)){

					//Calculating cost
					gtemp = details[i][j].g + 1.414;
					htemp = getHeuristic(i + 1, j - 1, dest.first, dest.second);
					ftemp = gtemp + htemp;

					//Check if never entered in open list or new cost is lower
					if((details[i + 1][j - 1].f  == FLOAT_MAX) || (details[i + 1][j - 1].f  > ftemp)){
						openList.insert(make_pair(ftemp, make_pair(i + 1, j - 1)));
						
						details[i + 1][j - 1].f = ftemp;
						details[i + 1][j - 1].g = gtemp;
						details[i + 1][j - 1].h = htemp;
						details[i + 1][j - 1].parent_i = i;
						details[i + 1][j - 1].parent_j = j;
					}
				}
			}
		}

		if(foundDest == false){
			printf("A* ERROR: Path not found.\n");
		}

	} else {
		printf("A* ERROR: Not initalized\n");
	}

	return;
}


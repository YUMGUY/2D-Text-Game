#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"


using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {

    // create 2D map array
    maxRow = 0;
    maxCol = 0;
    string Line_Data = "";
    bool exit_exist = false;

    ifstream filelol(fileName);

    // case where file doesn't exist or cannot open
    if(!filelol.is_open()) {
        return nullptr;
    }

    // check for if values are valid
    filelol >> maxRow ;
    if(filelol.fail()) {
        return nullptr;
    }
    if(maxRow < 1) {
        return nullptr;
    }
    
    
    filelol >> maxCol;
    if(filelol.fail()) {
        return nullptr;
    }

    if(maxCol < 1) {
        return nullptr;
    }

    // check amt of tiles
    if(maxCol > INT32_MAX/maxRow) {
        return nullptr;
    }


    filelol >> player.row;
    if(filelol.fail()) {
        return nullptr;
    }
    if(player.row >= maxRow || player.row < 0) {
        return nullptr;
    }

    filelol >> player.col;
    if(filelol.fail()) {
        return nullptr;
    }
    if(player.col >= maxCol || player.col < 0) {
        return nullptr;
    }

    
    char** created_Map = createMap(maxRow, maxCol);

    for(int i = 0; i < maxRow; ++i) {

        for(int j = 0; j < maxCol; ++j) {

            filelol >> created_Map[i][j];

            // check if valid char was read/ sufficient tiles
            if(filelol.fail()) {
                deleteMap(created_Map,maxRow);
                return nullptr;
            }

            //check if player tile already exits, then overwrite with open tile
            if(created_Map[i][j] == TILE_PLAYER) {
                created_Map[i][j] = TILE_OPEN;
            }

            // check if valid tile
            if(created_Map[i][j] != TILE_PILLAR && created_Map[i][j] != TILE_OPEN && created_Map[i][j] != TILE_TREASURE && created_Map[i][j] != TILE_AMULET && created_Map[i][j] != TILE_MONSTER && created_Map[i][j] != TILE_DOOR && created_Map[i][j] != TILE_EXIT) {
    
                deleteMap(created_Map,maxRow);
                return nullptr;
            }

            if(created_Map[i][j] == TILE_DOOR || created_Map[i][j] == TILE_EXIT) {


                exit_exist = true;
            }


        }
    }
    // check if door/exit exists
    if(!(exit_exist)) {
        deleteMap(created_Map,maxRow);
        return nullptr;
    }
       
   
    // check if number of tiles is greater than tiles read
    char test_var;
    filelol >> test_var;
    if(!filelol.fail()) {
        deleteMap(created_Map,maxRow);
        return nullptr;
    }

    // putting the player in the map
    created_Map[player.row][player.col] = TILE_PLAYER;


    return created_Map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {

    if(input == 'w') {
        --nextRow;
    }
    else if(input == 'a') {
        --nextCol;
    }
    else if(input == 's') {
        ++nextRow;
    }
    else if(input == 'd') {
        ++nextCol;
    }
    else {
        nextRow = nextRow;
        nextCol = nextCol;
    }
    
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {



    if(maxRow < 1 || maxCol < 1) {
        return nullptr;
    }

    char** newMap = new char*[maxRow];

    for(int i = 0; i < maxRow; ++i) {

        newMap[i] = new char[maxCol];

        for(int j = 0; j < maxCol; ++j) {
            newMap[i][j] = TILE_OPEN;
        }
    }
    
    return newMap;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {

    //
    if(map == nullptr) {
        map = nullptr;
    }

    else {
    for(size_t i = 0; i < maxRow; ++i) {
        delete[] map[i];
    }
    delete[] map;
    }
    map = nullptr;
    maxRow = 0;
    
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {

    // unhappy paths
    if(map == nullptr) {
        return nullptr;
    }

    if(maxRow <= 0 || maxCol <= 0) {
        return nullptr;
    }


    
    maxRow *= 2;
    maxCol *= 2;

    char** new_sizeMap = new char*[maxRow];
    for(size_t i = 0; i < maxRow; ++i) {
        new_sizeMap[i] = new char[maxCol];
    }
// e.g old map is 3x3, new map is 6x6

    // first quadrant A

    for(size_t i = 0; i < maxRow/2; ++i) {
        for(size_t j = 0; j < maxCol/2; ++j) {      //3x3 
            new_sizeMap[i][j] = map[i][j]; 

        }
    }

    // second quadrant B
    for(size_t i = 0; i < maxRow/2; ++i) {
        for(size_t j = maxCol/2; j < maxCol; ++j) {   // j = 5 < 6

            if(map[i][j - maxCol/2] == TILE_PLAYER) {
                new_sizeMap[i][j] = TILE_OPEN;
                continue;
            }

            new_sizeMap[i][j] = map[i][j - maxCol/2];

        }
    }

    // third quadrant C
    for(size_t i = maxRow/2; i < maxRow; ++i) {
        for(size_t j = 0; j < maxCol/2; ++j) {   // j = 5 < 6

            if(map[i- maxRow/2][j] == TILE_PLAYER) {
                new_sizeMap[i][j] = TILE_OPEN;
                continue;
            
            }
            new_sizeMap[i][j] = map[i - maxRow/2][j];
        }
    }

    // fourth quadrant D
     for(size_t i = maxRow/2; i < maxRow; ++i) {
        for(size_t j = maxCol/2; j < maxCol; ++j) {   // j = 5 < 6

          if(map[i- maxRow/2][j - maxCol/2] == TILE_PLAYER) {
                new_sizeMap[i][j] = TILE_OPEN;
                continue;
            }
            new_sizeMap[i][j] = map[i - maxRow/2][j - maxCol/2];
        }
    }

    for(size_t i = 0; i < maxRow/2; ++i) {
        delete[] map[i];
    }
    delete[] map;
    

    return new_sizeMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {



    // check for unhappy moving out of bounds or moving towards impassible object
    if(nextRow < 0 || nextCol < 0 || nextRow >= maxRow || nextCol >= maxCol || map[nextRow][nextCol] == TILE_PILLAR) {
        return STATUS_STAY;
    }

    if(map[nextRow][nextCol] == TILE_EXIT && player.treasure <= 0) {
        return STATUS_STAY;
    }

    // start of happy path

    if(map[nextRow][nextCol] == TILE_EXIT && player.treasure >= 1) {
         map[player.row][player.col] = TILE_OPEN; // the old spot
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_ESCAPE;
    }

    if(map[nextRow][nextCol] == TILE_TREASURE) {
        map[player.row][player.col] = TILE_OPEN; // the old spot
        map[nextRow][nextCol] = TILE_PLAYER;
        player.treasure += 1;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_TREASURE;
    }
    if(map[nextRow][nextCol] == TILE_AMULET) {
        map[player.row][player.col] = TILE_OPEN; // the old spot
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_AMULET;
    }
    if(map[nextRow][nextCol] == TILE_MONSTER) { 
        
        return STATUS_STAY;
    }

    if(map[nextRow][nextCol] == TILE_OPEN) {
        map[player.row][player.col] = TILE_OPEN; // the old spot
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_MOVE;
    }

    if(map[nextRow][nextCol] == TILE_DOOR) {
        map[player.row][player.col] = TILE_OPEN; // the old spot
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_LEAVE;
    }

    // // default
    // return STATUS_STAY;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {

    bool pillar_block = false;
    bool monster_attack = false; // so that monsters move in unison

    // check up
    for(int i = player.row - 1; i >= 0; --i) {

        if(map[i][player.col] == TILE_PILLAR) {
            pillar_block = true;
        }

        if(map[i][player.col] == TILE_MONSTER && !pillar_block) {  // spotted by monster without pillar
            

            map[i][player.col] = TILE_OPEN; 
            map[i+1][player.col] = TILE_MONSTER;
            
            if(map[player.row][player.col] == map[i+1][player.col]) {
                monster_attack = true;
            }
            
        }
    }
    
    // reset pillar block for next checks 
    pillar_block = false;

   // check down 
    for(int i = player.row + 1; i < maxRow; ++i) {

        if(map[i][player.col] == TILE_PILLAR) {
            pillar_block = true;
        }

        if(map[i][player.col] == TILE_MONSTER && !pillar_block) {  
            
            map[i][player.col] = TILE_OPEN; 
            map[i - 1][player.col] = TILE_MONSTER;


            if(map[player.row][player.col] == map[i - 1][player.col]) {
                monster_attack = true;
            }
       }
        
   } 
   
    pillar_block = false;
    // check left

    for(int i = player.col - 1; i >= 0; --i) {

        if(map[player.row][i] == TILE_PILLAR) {
            pillar_block = true;
        }

        if(map[player.row][i] == TILE_MONSTER && !pillar_block) {
          
            map[player.row][i] = TILE_OPEN;
            map[player.row][i + 1] = TILE_MONSTER;

            if(map[player.row][player.col] == map[player.row][i + 1]) {
                monster_attack = true;
            }
        }
    }

    pillar_block = false;

    // check right
    for(int i = player.col + 1; i < maxCol; ++i) {
     

        if(map[player.row][i] == TILE_PILLAR) {
            pillar_block = true;
        }
        
        if(map[player.row][i] == TILE_MONSTER && !pillar_block) {
           
            map[player.row][i] = TILE_OPEN;
            map[player.row][i - 1] = TILE_MONSTER; 
            
            if(map[player.row][player.col] == map[player.row][i - 1]) {
                monster_attack = true;
            }
        }
    }
        
        
    
    // once for loop finishes without problem
    return monster_attack; // return monster_attack
}

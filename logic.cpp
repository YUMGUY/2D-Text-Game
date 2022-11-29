#include <iostream>
#include <fstream>
#include <string>
#include "textLogic.h"


using std::cout, std::endl, std::ifstream, std::string;

char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {

    // create 2D map array
    maxRow = 0;
    maxCol = 0;
    string Line_Data = "";
    bool exit_exist = false;

    std::cout << fileName << std::endl;
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

}

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

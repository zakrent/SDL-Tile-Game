//
// Created by zakrent on 3/18/17.
//

#include "Map.h"


void Map::generateMap() {
    //temp
    for (int i = 0; i < MAP_WIDTH; ++i) {
        for (int a = 0; a < MAP_HEIGHT; ++a) {
            tiles.push_back(Tile(i, a, 0, MAP_HEIGHT*i+a));
        }
    }
    tiles[21].type = 1;// debug
    tiles[23].type = 1;// debug
    entities.push_back(Entity(0,32,32));
    entities.push_back(Entity(0,10,0));
    entities.push_back(Entity(0,20,64));
    entities.push_back(Entity(0,0,38));
    players.push_back(Player(0,64,320));
    //TODO: Remove debug
}

void Map::updateEntities() {
    for(Entity &entity : entities){
        entity.update();
    }
    for(Player &player : players){
        player.update();
    }
}
bool isPointInRectangle(SDL_Rect& collider, int pointX, int pointY){
    return bool
            (abs(pointX) >= abs(collider.x)            &&
             abs(pointX) <= abs(collider.x+collider.w) &&
             abs(pointY) >= abs(collider.y)            &&
             abs(pointY) <= abs(collider.y+collider.h));

}
bool Map::checkIfCollidersColide(SDL_Rect& colliderA, SDL_Rect& colliderB) {
    return bool
            (isPointInRectangle(colliderA, colliderB.x+1, colliderB.y+1)             ||
             isPointInRectangle(colliderA, colliderB.x+colliderB.w-1, colliderB.y+1) ||
             isPointInRectangle(colliderA, colliderB.x+1, colliderB.y+colliderB.h-1) ||
             isPointInRectangle(colliderA, colliderB.x+colliderB.w-1, colliderB.y+colliderB.h-1)
            );
}

void Map::checkCollisions() {
    for(Entity &entity : entities){
        for(Tile &tile : tiles){
            if(checkIfCollidersColide(entity.collider, tile.collider)){
                entity.handleTileCollision(tile);
            }
        }
    }
    for(Entity &player : players){
        for(Tile &tile : tiles){
            if(checkIfCollidersColide(player.collider, tile.collider)){
                player.handleTileCollision(tile);
            }
        }
    }
}

int Map::getNeighbouringTileIndex(direction directionToNeighbour, Tile mainTile) {
    int neighbourTileIndex;
    bool doesNeighbourExist = (
            mainTile.index != NULL && (
                                              (directionToNeighbour == Up && mainTile.index % MAP_HEIGHT != 0) ||
                                              (directionToNeighbour == Down && mainTile.index % MAP_HEIGHT != MAP_HEIGHT-1) ||
                                              (directionToNeighbour == Right && mainTile.index < (MAP_HEIGHT * MAP_WIDTH)-MAP_HEIGHT) ||
                                              (directionToNeighbour == Left && mainTile.index > MAP_HEIGHT-1)
                                      )
    );
    if(!doesNeighbourExist){
        neighbourTileIndex = NULL;
    }else{
        switch(directionToNeighbour){
            case Up:
                neighbourTileIndex = mainTile.index-1;
                break;
            case Down:
                neighbourTileIndex = mainTile.index+1;
                break;
            case Right:
                neighbourTileIndex = mainTile.index+MAP_HEIGHT;
                break;
            case Left:
                neighbourTileIndex = mainTile.index-MAP_HEIGHT;
        }
    }
    return neighbourTileIndex;
}


void Map::render(SDL_Renderer& renderer, SDL_Rect& camera, SDL_Texture* TileSheet, SDL_Texture* EntitySheet) {
    for (Tile &tile : tiles) {
        if(checkIfCollidersColide(camera, tile.collider)){
            tile.render(renderer, camera, TileSheet);
        }
    }
    for(Entity &entity : entities){
        entity.render(renderer, camera, EntitySheet);
    }
    for(Player &player : players){
        player.render(renderer, camera, EntitySheet);
    }

}

Map::Map() {
    generateMap();
}



#pragma once
#include "..\sdl\include\SDL2\SDL.h"
#include "..\sdl\include\SDL2\SDL_image.h"
#include "Actor.h"
#include "SpriteComponent.h"

/*
*Tiles are used to position the Tower and to give Enemies a route
*/
class Tile : public Actor
{
public:
  friend class Grid;

  Tile(class Game *game);
  enum TileState
  {
    EDefault,
    EPath,
    EStart,
    EBase
  };

  void UpdateTile();

  void SetTileState(TileState state) { tState = state; }
  TileState GetTileState() const { return tState; }

  Tile *GetParent() const { return mParent; }

  SpriteComponent *mSprite;

  //The attributes f, g, h, mInOpenSet, mInClosedSet and mBlocked are used to implement the A* algorithm
private:
  float f;
  float g;
  float h;
  bool mInOpenSet;
  bool mInClosedSet;
  bool mBlocked;
  Tile *mParent;
  std::vector<Tile *> mAdjacent;

  TileState tState;
  void UpdateTexture();
};
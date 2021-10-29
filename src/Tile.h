#pragma once
#include <SDL2/SDL.h>
#include "Actor.h"
#include "SpriteComponent.h"

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

  void GenerateOutput(SDL_Renderer *renderer);
  SpriteComponent *mSprite;

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
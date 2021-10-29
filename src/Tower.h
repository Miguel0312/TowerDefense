#include "Game.h"
#include "Grid.h"
#include "Actor.h"

class Tower : public Actor
{
public:
  Tower(Game *game, Grid *grid, Tile *tile);

  void GenerateOutput(SDL_Renderer *renderer);
  void UpdateActor(float deltaTime) override;
  class Enemy *ClosestEnemy();

private:
  Grid *mGrid;
  SpriteComponent *mSprite;
  float angVel;
  float relTime;
};
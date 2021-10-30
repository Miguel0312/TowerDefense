#include "Game.h"
#include "Grid.h"
#include "Actor.h"

/*
*A Tower is placed at a Tile and shoots projectiles at its target
*/
class Tower : public Actor
{
public:
  Tower(Game *game, Grid *grid, Tile *tile);

  void UpdateActor(float deltaTime) override;
  class Enemy *ClosestEnemy();

private:
  Grid *mGrid;
  SpriteComponent *mSprite;
  float angVel;
  float relTime;
};
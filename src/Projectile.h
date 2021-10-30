#include "Actor.h"
#include "Grid.h"
#include "SpriteComponent.h"

/*
*The projectile is fired by a Tower and follows an Enemy
*/
class Projectile : public Actor
{
public:
  Projectile(Game *game, Grid *grid, Enemy *target);
  ~Projectile();

  void UpdateActor(float deltaTime);

private:
  Grid *mGrid;
  class Enemy *mTarget;
  float linVel;
  SpriteComponent *mSprite;
};
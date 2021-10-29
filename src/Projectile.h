#include "Actor.h"
#include "Grid.h"
#include "SpriteComponent.h"

class Projectile : public Actor
{
public:
  Projectile(Game *game, Grid *grid, Enemy *target);
  void UpdateActor(float deltaTime);
  void GenerateOutput(SDL_Renderer *renderer);

private:
  Grid *mGrid;
  class Enemy *mTarget;
  float linVel;
  SpriteComponent *mSprite;
};
#include "Actor.h"
#include "Grid.h"
#include "Tile.h"

class Enemy : public Actor
{
public:
  Enemy(Game *game, Grid *grid);

  void UpdateActor(float deltaTime) override;
  void GenerateOutput(SDL_Renderer *renderer);

  void MakeDamage(int damage) { HP -= damage; }

private:
  SpriteComponent *mSprite;
  Grid *mGrid;
  int linVel;
  float angVel;
  Tile *target;
  int HP;
};
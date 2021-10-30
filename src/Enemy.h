#include "Actor.h"
#include "Grid.h"
#include "Tile.h"

/*
*Enemy is the object that the player has to destroy using the towers
*It has an HP that will signal when it is killed, and a Tile target, that is the next Tile to which it is currently heading
*/
class Enemy : public Actor
{
public:
  Enemy(Game *game, Grid *grid);
  ~Enemy();

  void UpdateActor(float deltaTime) override;

  void MakeDamage(int damage) { HP -= damage; }

private:
  SpriteComponent *mSprite;
  Grid *mGrid;
  int linVel;
  float angVel;
  Tile *target;
  int HP;
};
#include "Tower.h"
#include "Enemy.h"

const float RELOAD_TIME = 3.0f;
const int RANGE = 300;

/*
*Creates a new Tower in a Tile
*/
Tower::Tower(Game *game, Grid *grid, Tile *tile) : Actor(game)
{
  mGrid = grid;
  SetPosition(tile->GetPosition());
  SetState(State::EPaused);

  mSprite = new SpriteComponent(this, 70);
  mSprite->SetTexture(game->GetTexture("Tower"));

  relTime = 0;
}

/*
*Finds the closest enemy and sets it as the target
*/
class Enemy *Tower::ClosestEnemy()
{
  float distSq = 10000000.0f;
  int index = -1, count = 0;
  for (auto enemy : mGrid->GetEnemies())
  {
    float temp = (GetPosition() - enemy->GetPosition()).LengthSq();
    if (temp < distSq)
    {
      index = count;
      distSq = temp;
    }
    count++;
  }
  if (index != -1)
    return mGrid->GetEnemies()[index];
  return nullptr;
}

/*
*Rotate the Tower such that it is aligned to its target
*If it has a close target, and it has reloaded, shoot a Projectile at it
*/
void Tower::UpdateActor(float deltaTime)
{
  Enemy *target = ClosestEnemy();

  if (target)
  {
    Vector2 targetVector = (GetPosition() - target->GetPosition());
    if (targetVector.LengthSq() < pow(RANGE, 2))
    {
      SetRotation(Math::Pi - atan2f(targetVector.y, targetVector.x));
      if (relTime < 0)
      {
        mGrid->AddProjectile(this);
        relTime = RELOAD_TIME;
      }
    }
  }

  relTime -= deltaTime;
}
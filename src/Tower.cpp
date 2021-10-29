#include "Tower.h"
#include "Enemy.h"

const float PI = 3.14159;

Tower::Tower(Game *game, Grid *grid, Tile *tile) : Actor(game)
{
  mGrid = grid;
  SetPosition(tile->GetPosition());

  mSprite = new SpriteComponent(this, 70);
  mSprite->SetTexture(game->GetTexture("Tower"));

  relTime = 0;
}

void Tower::GenerateOutput(SDL_Renderer *renderer)
{
  mSprite->Draw(renderer);
}

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

void Tower::UpdateActor(float deltaTime)
{
  Enemy *target = ClosestEnemy();

  if (target)
  {
    Vector2 targetVector = (GetPosition() - target->GetPosition());
    if (targetVector.LengthSq() < 300 * 300)
    {
      SetRotation(PI - atan2f(targetVector.y, targetVector.x));
      if (relTime < 0)
      {
        mGrid->AddProjectile(this);
        relTime = 3.0f;
      }
    }
  }

  relTime -= deltaTime;
}
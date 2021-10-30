#include "Enemy.h"

/*
*The constructor places the Enemy at the beginning of the path
*Also, it determines the speed and the HP of the enemy
*/
Enemy::Enemy(Game *game, Grid *grid) : Actor(game)
{
  mGrid = grid;

  target = grid->GetBegin();
  SetPosition(target->GetPosition());

  mSprite = new SpriteComponent(this, 100);
  mSprite->SetTexture(game->GetTexture("Airplane"));

  linVel = 100;
  angVel = 1.0f;

  HP = 5;
}

/*
*In addition to the default operations made to delete an actor, Enemy is also removed from the Grid where it is stored
*/
Enemy::~Enemy()
{
  mGrid->RemoveEnemy(this);
}

/*
*If the Actor gets to the center of its target's center, the target is updated to the next Tile, or, if it gets to the end of the path, its State is set to Dead, so it will be destroyed.
*If its HP gets to 0, we aksi set the State to Dead
*If we set a new target, the new velocity vector is determined
*/
void Enemy::UpdateActor(float deltaTime)
{
  if ((target->GetPosition() - this->GetPosition()).Length() < 3)
  {
    if (target == mGrid->GetEnd())
      SetState(Actor::State::EDead);
    else
      target = target->GetParent();
  }

  if (HP <= 0)
  {
    SetState(Actor::State::EDead);
  }

  Vector2 targetPosition = target->GetPosition();
  float tan = (targetPosition.y - mPosition.y) / abs(targetPosition.x - mPosition.x);
  float ang = -atan(tan);
  mRotation = ang;

  mPosition.x += linVel * deltaTime * cos(mRotation);
  mPosition.y -= linVel * deltaTime * sin(mRotation);
}
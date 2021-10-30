#include "Enemy.h"

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

Enemy::~Enemy()
{
  mGrid->RemoveEnemy(this);
}

void Enemy::GenerateOutput(SDL_Renderer *renderer)
{
  mSprite->Draw(renderer);
}

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
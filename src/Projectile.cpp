#include "Projectile.h"
#include "Enemy.h"

const float PI = 3.14159;

Projectile::Projectile(Game *game, Grid *grid, Enemy *enemy) : Actor(game)
{
  mGrid = grid;
  mTarget = enemy;

  mSprite = new SpriteComponent(this, 100);
  mSprite->SetTexture(game->GetTexture("Projectile"));
  linVel = 300;
}

void Projectile::UpdateActor(float deltaTime)
{
  if (mTarget->GetState() == Actor::State::EDead)
  {
    SetState(Actor::State::EDead);
    return;
  }

  Vector2 targetVector = (GetPosition() - mTarget->GetPosition());
  SetRotation(PI - atan2f(targetVector.y, targetVector.x));

  if (targetVector.LengthSq() < 15 * 15)
  {
    SetState(Actor::State::EDead);
    mTarget->MakeDamage(1);
  }

  mPosition.x += linVel * deltaTime * cos(mRotation);
  mPosition.y -= linVel * deltaTime * sin(mRotation);
}

void Projectile::GenerateOutput(SDL_Renderer *renderer)
{
  mSprite->Draw(renderer);
}
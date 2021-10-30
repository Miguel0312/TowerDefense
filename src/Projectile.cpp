#include "Projectile.h"
#include "Enemy.h"

const int SPEED = 300;
const int HITBOX_RADIUS = 15;

/*
*Sets up the Enmy that will be the target of the Projectile
*/
Projectile::Projectile(Game *game, Grid *grid, Enemy *enemy) : Actor(game)
{
  mGrid = grid;
  mTarget = enemy;

  mSprite = new SpriteComponent(this, 100);
  mSprite->SetTexture(game->GetTexture("Projectile"));
  linVel = SPEED;
}

Projectile::~Projectile()
{
  mGrid->RemoveProjectile(this);
}

/*
*The Projectile advances through he current line connecting it and its target
*If it is close enough, it makes damage and is destroyed
*If the Enemy that the Projectile is following is destroyed, it is also deleted
*/
void Projectile::UpdateActor(float deltaTime)
{
  if (mTarget->GetState() == Actor::State::EDead)
  {
    SetState(Actor::State::EDead);
    return;
  }

  Vector2 targetVector = (GetPosition() - mTarget->GetPosition());
  SetRotation(Math::Pi - atan2f(targetVector.y, targetVector.x));

  if (targetVector.LengthSq() < pow(HITBOX_RADIUS, 2))
  {
    SetState(Actor::State::EDead);
    mTarget->MakeDamage(1);
  }

  mPosition.x += linVel * deltaTime * cos(mRotation);
  mPosition.y -= linVel * deltaTime * sin(mRotation);
}
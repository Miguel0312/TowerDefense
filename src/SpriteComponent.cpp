#include "SpriteComponent.h"
#include "Math.h"

SpriteComponent::SpriteComponent(Actor *actor, int drawOrder) : Component(actor)
{
  mDrawOrder = drawOrder;
}

void SpriteComponent::SetTexture(SDL_Texture *texture)
{
  mTexture = texture;
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

void SpriteComponent::Draw(SDL_Renderer *renderer)
{
  if (mTexture)
  {
    SDL_Rect r;
    r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);
    //SDL_Log("%d\t%d", r.w, r.h);

    SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
  }
  // SDL_Log("test");
}
#include "AnimSpriteComponent.h"

void AnimSpriteComponent::SetAnimTextures(std::vector<SDL_Texture *> textures)
{
  mAnimTextures = textures;
  mCurrFrame = 0;
  SetTexture(mAnimTextures[mCurrFrame]);
}

void AnimSpriteComponent::Update(float deltaTime)
{
  SpriteComponent::Update(deltaTime);
  if (mAnimTextures.size() > 0)
  {
    mCurrFrame += mAnimFPS * deltaTime;
    while (mCurrFrame >= mAnimTextures.size())
    {
      mCurrFrame -= mAnimTextures.size();
    }
    SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
  }
}
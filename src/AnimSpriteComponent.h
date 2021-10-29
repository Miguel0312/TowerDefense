#pragma once
#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent
{
public:
  AnimSpriteComponent(class Actor *owner, int drawOrder) : SpriteComponent(owner, drawOrder){};

  void Update(float deltaTime) override;
  void SetAnimTextures(const std::vector<SDL_Texture *> textures);

  float GetAnimFPS() const { return mAnimFPS; }
  void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
  std::vector<SDL_Texture *> mAnimTextures;
  float mCurrFrame;
  float mAnimFPS;
};
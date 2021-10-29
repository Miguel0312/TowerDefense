#pragma once
#include "SpriteComponent.h"

class BGSpriteComponent : public SpriteComponent
{
public:
  BGSpriteComponent(class Actor *owner, int drawOrder = 1) : SpriteComponent(owner, drawOrder){};

  void Update(float deltaTime) override;
  void Draw(SDL_Renderer *renderer) override;

  void SetBGTextures(const std::vector<SDL_Texture *> &textures, int rows, int columns, Vector2 tileSize);

  void SetTileSize(Vector2 tileSize) { mTileSize = tileSize; }

private:
  struct BGTexture
  {
    SDL_Texture *mTexture;
    Vector2 mOffset;
  };
  std::vector<BGTexture> mBGTextures;
  Vector2 mTileSize;
};
#include "BGSpriteComponent.h"

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture *> &textures, int rows, int columns, Vector2 tileSize)
{
  mTileSize = tileSize;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      BGTexture temp;
      temp.mTexture = textures[columns * i + j];

      temp.mOffset.x = j * tileSize.x;
      temp.mOffset.y = i * tileSize.y;

      mBGTextures.emplace_back(temp);
    }
  }
}

void BGSpriteComponent::Update(float deltaTime)
{
}

void BGSpriteComponent::Draw(SDL_Renderer *renderer)
{
  for (auto &bg : mBGTextures)
  {
    SDL_Rect r;
    r.x = bg.mOffset.x;
    r.y = bg.mOffset.y;
    r.w = static_cast<int>(mTileSize.x);
    r.h = static_cast<int>(mTileSize.y);
    SDL_RenderCopy(renderer, bg.mTexture, nullptr, &r);
  }
}
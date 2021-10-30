#pragma once
#include "Component.h"
#include "Actor.h"
#include "..\sdl\include\SDL2\SDL.h"
#include "..\sdl\include\SDL2\SDL_image.h"

/*
*Sprite Component allows an Actor to have an image displayed in the screen
*/
class SpriteComponent : public Component
{
public:
  SpriteComponent(class Actor *owner, int drawOrder = 100);
  ~SpriteComponent();

  virtual void Draw(SDL_Renderer *renderer);
  virtual void SetTexture(SDL_Texture *texture);

  int GetDrawOrder() const { return mDrawOrder; }
  int GetTexHeight() const { return mTexHeight; }
  int GetTexWidth() const { return mTexWidth; }

  void SetTexSize(Vector2 dimensions)
  {
    mTexWidth = dimensions.x;
    mTexHeight = dimensions.y;
  }

protected:
  SDL_Texture *mTexture;
  int mDrawOrder;
  int mTexWidth;
  int mTexHeight;
};
#include "SpriteComponent.h"
#include "Math.h"

/*
*Sets up the DrawOrder and adds itself to the Game's sprites
*/
SpriteComponent::SpriteComponent(Actor *actor, int drawOrder) : Component(actor)
{
  mDrawOrder = drawOrder;
  mOwner->GetGame()->AddSprite(this);
}

/*
*In addition to the Component's default destructor, the Sprite has to be removed from the list of Sprites of the Game
*/
SpriteComponent::~SpriteComponent()
{
  mOwner->GetGame()->RemoveSprite(this);
}

/*
*Sets the texture of the component
*/
void SpriteComponent::SetTexture(SDL_Texture *texture)
{
  mTexture = texture;
  //This function stores the texture's size in the mTexWidth and mTexHeight variables
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

/*
*This function draws the Sprite
*Notice that Actor stores its central coordinates, while the SDL_Rect (x,y) coordinates are the location of its top-left corner
*/
void SpriteComponent::Draw(SDL_Renderer *renderer)
{
  if (mTexture)
  {
    SDL_Rect r;
    r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

    //SDL_RenderCopyEx has to be used to allow rotations of the Texture
    //The angle argument has to be given in degrees in the clockwise direction
    SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
  }
}
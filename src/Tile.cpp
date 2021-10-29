#include "Tile.h"

Tile::Tile(class Game *game) : Actor(game)
{
  tState = EDefault;
  mSprite = new SpriteComponent(this, 10);
  mBlocked = false;
  UpdateTexture();
}

void Tile::GenerateOutput(SDL_Renderer *renderer)
{
  mSprite->Draw(renderer);
}

void Tile::UpdateTexture()
{
  std::string text;
  switch (tState)
  {
  case EStart:
    text = "TileTan";
    break;
  case EBase:
    text = "TileGreen";
    break;
  case EPath:
    text = "TileGrey";
    break;
  case EDefault:
  default:
    text = "TileBrown";
    break;
  }
  mSprite->SetTexture(GetGame()->GetTexture(text));
}

void Tile::UpdateTile()
{
  UpdateTexture();
}
#include "Tile.h"

/*
*Sets up the State and Texture of the Tile
*/
Tile::Tile(class Game *game) : Actor(game)
{
  tState = EDefault;
  mSprite = new SpriteComponent(this, 10);
  mBlocked = false;
  UpdateTexture();
}

/*
*Determines the texture of the Tile based on its current status
*/
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
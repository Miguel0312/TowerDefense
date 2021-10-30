#pragma once
#include "..\sdl\include\SDL2\SDL.h"
#include "..\sdl\include\SDL2\SDL_image.h"
#include <vector>
#include <map>
#include <string>

/*
*Game is the principal class of the program.
*It stores all the Actors and Sprites
*It is responsible to initializing, updating and close the game
*/
class Game
{
public:
  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

  SDL_Texture *GetTexture(std::string textureName);

  void AddActor(class Actor *actor);
  void RemoveActor(class Actor *actor);

  void AddSprite(class SpriteComponent *sprite);
  void RemoveSprite(class SpriteComponent *sprite);

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  void LoadTexture(const char *fileName, const char *textureName);

  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;

  bool mIsRunning;
  bool mUpdatingActors;
  Uint32 mTicksCount;
  int width;
  int height;

  std::vector<Actor *> mActors;
  std::vector<Actor *> mPendingActors;
  std::vector<SpriteComponent *> mSprites;

  class Grid *mGrid;

  std::map<std::string, SDL_Texture *> mTextures;
};
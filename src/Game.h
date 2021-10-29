#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>

class Game
{
public:
  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

  SDL_Texture *GetTexture(std::string textureName);

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  void AddActor(class Actor *actor);
  void RemoveActor(class Actor *actor);

  void LoadTexture(const char *fileName, const char *textureName);

  void AddSprite(class SpriteComponent *sprite);

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
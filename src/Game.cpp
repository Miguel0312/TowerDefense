#include <bits/stdc++.h>
#include <string>
#include "Game.h"
#include "Actor.h"
#include "Grid.h"
#include "Enemy.h"

Game::Game()
{
  mIsRunning = true;
  mTicksCount = 0;
  width = 1024;
  height = 704;
}

bool Game::Initialize()
{
  int SDLResult = SDL_Init(SDL_INIT_VIDEO);
  if (SDLResult != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  IMG_Init(IMG_INIT_PNG);

  mWindow = SDL_CreateWindow("Tower Defense", 100, 0, width, height, 0);
  if (!mWindow)
  {
    SDL_Log("Unable to create SDL window: %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!mRenderer)
  {
    SDL_Log("Unable to create SDL renderer: %s", SDL_GetError());
    return false;
  }

  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/TileBrown.png", "TileBrown");
  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/TileGreen.png", "TileGreen");
  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/TileGrey.png", "TileGrey");
  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/TileTan.png", "TileTan");

  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/Airplane.png", "Airplane");
  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/Tower.png", "Tower");
  LoadTexture("/home/miguel/Desktop/Books/Programming/Game Development/Tower Defense/src/include/Projectile.png", "Projectile");

  mGrid = new Grid(this, 11, 16);

  mGrid->FindPath(mGrid->GetBegin(), mGrid->GetEnd());
  mGrid->UpdatePathTiles(mGrid->GetBegin(), mGrid->GetEnd());
  mGrid->UpdateActor(0);

  return true;
}

void Game::Shutdown()
{
  while (!mActors.empty())
  {
    delete mActors.back();
  }
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::RunLoop()
{
  while (mIsRunning)
  {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::ProcessInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      mIsRunning = false;
      break;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_ESCAPE])
  {
    mIsRunning = false;
  }

  int x, y;
  Uint32 buttons = SDL_GetMouseState(&x, &y);

  if (!mGrid->IsRunning())
  {
    if (SDL_BUTTON(buttons) & 1)
    {
      //SDL_Log("Yes");
      mGrid->AddTower(Vector2(x, y));
    }
    if (buttons & 4)
    {
      //SDL_Log("Yes2");
      mGrid->Begin();
    }
  }
}

void Game::UpdateGame()
{
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
  if (deltaTime > 0.05)
    deltaTime = 0.05;

  for (auto sprite : mSprites)
  {
    sprite->Update(deltaTime);
  }

  mUpdatingActors = true;

  for (auto actor : mActors)
  {
    actor->Update(deltaTime);
  }

  mUpdatingActors = false;

  for (auto actor : mPendingActors)
  {
    mActors.emplace_back(actor);
  }
  mPendingActors.clear();

  std::vector<Actor *> deadActors;
  for (auto actor : mActors)
  {
    if (actor->GetState() == Actor::EDead)
    {
      deadActors.emplace_back(actor);
    }
  }

  for (auto actor : deadActors)
  {
    RemoveActor(actor);
  }

  mGrid->UpdateActor(deltaTime);

  mTicksCount = SDL_GetTicks();
}

void Game::GenerateOutput()
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
  SDL_RenderClear(mRenderer);

  for (auto sprite : mSprites)
  {
    sprite->Draw(mRenderer);
  }

  mGrid->GenerateOutput(mRenderer);

  SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor *actor)
{
  if (mUpdatingActors)
  {
    mPendingActors.emplace_back(actor);
  }
  else
  {
    mActors.emplace_back(actor);
  }
}

void Game::RemoveActor(Actor *actor)
{
  if (std::find(mActors.begin(), mActors.end(), actor) != mActors.end())
  {
    auto index = std::find(mActors.begin(), mActors.end(), actor);
    mActors.erase(index);
  }
  else if (std::find(mPendingActors.begin(), mPendingActors.end(), actor) != mPendingActors.end())
  {
    auto index = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    mPendingActors.erase(index);
  }
}

void Game::LoadTexture(const char *fileName, const char *textureName)
{
  SDL_Surface *surface = IMG_Load(fileName);
  if (!surface)
  {
    SDL_Log("Failed to load texture file %s", IMG_GetError());
    return;
  }
  SDL_Texture *text = SDL_CreateTextureFromSurface(mRenderer, surface);
  SDL_FreeSurface(surface);

  if (!text)
  {
    SDL_Log("Failed to load texture from surface %s", IMG_GetError());
    return;
  }

  mTextures[textureName] = text;
  if (mTextures[textureName] == nullptr)
    SDL_Log("Error");
}

SDL_Texture *Game::GetTexture(std::string textureName)
{
  for (auto it = mTextures.begin(); it != mTextures.end(); it++)
  {
    if (it->second == nullptr)
    {
      SDL_Log("Error");
    }
  }
  return mTextures[textureName];
}

void Game::AddSprite(SpriteComponent *sprite)
{
  int drawOrder = sprite->GetDrawOrder();
  auto iter = mSprites.begin();

  for (; iter != mSprites.end(); iter++)
  {
    if (drawOrder < (*iter)->GetDrawOrder())
      break;
  }

  mSprites.insert(iter, sprite);
}
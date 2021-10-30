#include <string>
#include <algorithm>
#include <iostream>
#include "Game.h"
#include "Actor.h"
#include "Grid.h"
#include "Enemy.h"

const int ROWS = 11;
const int COLUMNS = 16;
const int TILE_SIZE = 64;

Game::Game()
{
  mIsRunning = true;
  mTicksCount = 0;
  width = COLUMNS * TILE_SIZE;
  height = ROWS * TILE_SIZE;
}

/*
*In the initialization will initialize SDL and SDL_image and create a Window and a Renderer
*Then, it will load the Textures, so they can be accessed quickly during the programs execution
*Finally, it will create a Grid 
*/
bool Game::Initialize()
{
  int SDLResult = SDL_Init(SDL_INIT_VIDEO);
  if (SDLResult != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  IMG_Init(IMG_INIT_PNG);

  mWindow = SDL_CreateWindow("Tower Defense", 100, 100, width, height, 0);
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

  LoadTexture("src/assets/TileBrown.png", "TileBrown");
  LoadTexture("src/assets/TileGreen.png", "TileGreen");
  LoadTexture("src/assets/TileGrey.png", "TileGrey");
  LoadTexture("src/assets/TileTan.png", "TileTan");

  LoadTexture("src/assets/Airplane.png", "Airplane");
  LoadTexture("src/assets/Tower.png", "Tower");
  LoadTexture("src/assets/Projectile.png", "Projectile");

  mGrid = new Grid(this, ROWS, COLUMNS, TILE_SIZE);

  mGrid->FindPath(mGrid->GetBegin(), mGrid->GetEnd());
  mGrid->UpdatePathTiles(mGrid->GetBegin(), mGrid->GetEnd());

  return true;
}

/*
*Deletes the Actors, destroys the Window and quits SDL
*/
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

/*
*Main loop of the game
*/
void Game::RunLoop()
{
  while (mIsRunning)
  {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

/*
*The key commands for the Game are the following:
*Press "ESC" to quit
*Left click to add turrets before the game has started
*Right click to star the game
*/
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
      mGrid->AddTower(Vector2(x, y));
    }
    if (buttons & 4)
    {
      mGrid->Begin();
    }
  }
}

/*
*We first set the game to update every 16 milisseconds (60 FPS)
*Then, we update the Actors
*Finally, the Dead Actors are removed
*/
void Game::UpdateGame()
{
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
  if (deltaTime > 0.05)
    deltaTime = 0.05;

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
    delete actor;
  }

  mTicksCount = SDL_GetTicks();
}

/*
*Cleans the screen and draws the Sprites
*/
void Game::GenerateOutput()
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
  SDL_RenderClear(mRenderer);
  for (auto sprite : mSprites)
  {
    sprite->Draw(mRenderer);
  }

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
  // Is it in pending actors?
  auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
  if (iter != mPendingActors.end())
  {
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, mPendingActors.end() - 1);
    mPendingActors.pop_back();
  }

  // Is it in actors?
  iter = std::find(mActors.begin(), mActors.end(), actor);
  if (iter != mActors.end())
  {
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, mActors.end() - 1);
    mActors.pop_back();
  }
}

/*
*Loads the texture and save them in a std::map, so they can be quickly accessed during the program's execution
*/
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

/*
*Retrieves a texture
*/
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

void Game::RemoveSprite(SpriteComponent *sprite)
{
  auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
  if (iter != mSprites.end())
    mSprites.erase(iter);
}
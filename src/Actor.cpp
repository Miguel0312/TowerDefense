#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <iostream>

/*
*Default constructor for Actor, which saves to which game it is connected and set its status to EActive
*/
Actor::Actor(Game *game)
{
  mGame = game;
  mState = EActive;
  mScale = 1.0f;
  mRotation = 0.0f;
  game->AddActor(this);
}

Actor::~Actor()
{
  mGame->RemoveActor(this);
  while (!mComponents.empty())
  {
    delete mComponents.back();
  }
}

void Actor::Update(float deltaTime)
{
  UpdateActor(deltaTime);
  UpdateComponents(deltaTime);
}

/*
*Add a new Component to the Actor, inserting it in the correct position
*/
void Actor::AddComponent(Component *component)
{
  int target = component->GetUpdateOrder();
  auto iter = mComponents.begin();
  for (; iter != mComponents.end(); iter++)
  {
    if (target < (*iter)->GetUpdateOrder())
      break;
  }
  mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component *component)
{
  auto iter = std::find(mComponents.begin(), mComponents.end(), component);
  if (iter != mComponents.end())
  {
    mComponents.erase(iter);
  }
}

void Actor::UpdateComponents(float deltaTime)
{

  for (auto component : mComponents)
  {
    component->Update(deltaTime);
  }
}

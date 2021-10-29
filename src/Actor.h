#pragma once
#include <vector>
#include "Math.h"
#include "Game.h"
#include "Component.h"

class Actor
{
public:
  enum State
  {
    EActive,
    EPaused,
    EDead
  };

  Actor(class Game *game);
  virtual ~Actor();

  void Update(float deltaTime);
  void UpdateComponents(float deltaTime);
  virtual void UpdateActor(float deltaTime);

  State GetState() { return mState; }
  Vector2 GetPosition() { return mPosition; }
  float GetScale() { return mScale; }
  float GetRotation() { return mRotation; }
  std::vector<class Component *> GetComponents() { return mComponents; }
  class Game *GetGame() { return mGame; }

  void SetState(State state) { mState = state; }
  void SetPosition(Vector2 position) { mPosition = position; }
  void SetScale(float scale) { mScale = scale; }
  void SetRotation(float rotation) { mRotation = rotation; }

  void AddComponent(class Component *component);
  void RemoveComponent(class Component *component);

private:
  State mState;
  float mScale;
  std::vector<class Component *> mComponents;
  class Game *mGame;

protected:
  Vector2 mPosition;
  float mRotation;
};
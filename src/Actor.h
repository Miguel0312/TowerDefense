#pragma once
#include <vector>
#include "Math.h"
#include "Game.h"
#include "Component.h"

/*
*An Actor is an entity in the game. Initially, it has only a position in the game world. Other behaviors (sprites, user input, etc) are added by inserting Components.
*/
class Actor
{
public:
  /*
  *State is either active, paused(Actor doesn't update) or dead (which makes the Actor be deleted) 
  */
  enum State
  {
    EActive,
    EPaused,
    EDead
  };

  Actor(class Game *game);
  virtual ~Actor();

  void UpdateComponents(float deltaTime);

  /*
  *The non overridable method Update is the one that is called by the class Game
  *The overridable method UpdateActor alllows the user to add specific features without using components
  */
  void Update(float deltaTime);
  virtual void UpdateActor(float deltaTime){};

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
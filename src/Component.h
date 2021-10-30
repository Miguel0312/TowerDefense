#pragma once
#include "Game.h"
#include "Actor.h"

/*
*The Component adds different behaviors to the actors.
*
*The updateOrder variable determines which components are updated first (Components with lower updateOrder are updated first)
*/
class Component
{
public:
  Component(class Actor *owner, int updateOrder = 100);
  virtual ~Component();

  virtual void Update(float deltaTime){};
  int GetUpdateOrder() const { return mUpdateOrder; }

  class Actor *GetOwner() { return mOwner; }

protected:
  class Actor *mOwner;
  int mUpdateOrder;
};
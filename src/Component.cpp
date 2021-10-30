#include "Component.h"

/*
*The Component is added to its owner's list already in the constructor
*/
Component::Component(class Actor *owner, int updateOrder)
{
  mOwner = owner;
  mUpdateOrder = updateOrder;
  owner->AddComponent(this);
}

Component::~Component()
{
  mOwner->RemoveComponent(this);
}
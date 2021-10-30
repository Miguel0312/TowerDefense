#include "Component.h"

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
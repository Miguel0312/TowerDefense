#include "Component.h"

Component::Component(class Actor *owner, int updateOrder)
{
  mOwner = owner;
  mUpdateOrder = updateOrder;
}
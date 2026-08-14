#include "AbilityObject.h"

AbilityObject::AbilityObject()
{
	static ABILITY_ID_TYPE abilityIdCounter = 0;

	abilityID = ++abilityIdCounter;
}

AbilityObject::~AbilityObject()
{

}

void AbilityObject::SetOwnerPawn(std::weak_ptr<Pawn> newOwner)
{
	ownerPawn = newOwner;
}

#include "AbilityObject.h"

AbilityObject::AbilityObject(ABILITY_ID_TYPE id, int level)
	:abilityID(id)
	,abilityLevel(level)
{

}

AbilityObject::~AbilityObject()
{

}

void AbilityObject::Draw()
{

}

void AbilityObject::TriggerOn()
{
	bTrigger = true;
}

void AbilityObject::TriggerOff()
{
	bTrigger = false;
}

void AbilityObject::SetOwnerPawn(std::weak_ptr<Pawn> newOwner)
{
	ownerPawn = newOwner;
}

void AbilityObject::SetAbilityLevel(int level)
{
	abilityLevel = level;
}

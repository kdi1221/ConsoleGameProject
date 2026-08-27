#include "AbilityObject.h"

//Ability 객체 생성을 위한 헤더파일 포함
#include "PlayerAbility/AbilityShotSpiritBall.h"

std::unique_ptr<AbilityObject> AbilityObject::CreateNewAbility(const ABILITY_ID_TYPE abilityID, int abilityLevel)
{
	//TODO : abilityID에 따른 생성 Ability 구분 필요..
	switch (abilityID)
	{
	case 1:
		return std::make_unique<AbilityShotSpiritBall>(abilityID, abilityLevel);
	}

	return nullptr;
}


AbilityObject::AbilityObject(ABILITY_ID_TYPE id, int level)
	:abilityID(id)
	,abilityLevel(level)
{

}

AbilityObject::~AbilityObject()
{

}

void AbilityObject::Tick(float deltaTime)
{

}

void AbilityObject::Draw()
{

}

bool AbilityObject::CanActivateAbility() const
{
	return true;
}

void AbilityObject::ActivateAbility()
{
	
}

void AbilityObject::EndAbility(bool bCancelAbility)
{
	
}

void AbilityObject::CancelAbility()
{
	EndAbility(true);
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

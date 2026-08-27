#include "AbilitySystemComponent.h"
#include "Actor/Pawn/Pawn.h"
#include "Ability/Shooter/Projectile/AbilitySpiritBall.h"

AbilitySystemComponent::AbilitySystemComponent()
{

}

AbilitySystemComponent::~AbilitySystemComponent() = default;


void AbilitySystemComponent::PostTick(float deltaTime)
{
	super::PostTick(deltaTime);

	for (auto& iterMapAbility : mapAbilities)
	{
		AbilityObject* ability = iterMapAbility.second.get();
		if (!ability || !ability->IsTrigger())
		{
			continue;
		}

		ability->Tick(deltaTime);
	}
}

void AbilitySystemComponent::Draw()
{
	super::Draw();

	for (auto& iterMapAbility : mapAbilities)
	{
		AbilityObject* ability = iterMapAbility.second.get();
		if (!ability || !ability->IsTrigger())
		{
			continue;
		}

		ability->Draw();
	}
}

ABILITY_ID_TYPE AbilitySystemComponent::AddNewAbility(ABILITY_ID_TYPE abilityID, int abilityLevel)
{
	std::unique_ptr<AbilityObject> newAbility = AbilityObject::CreateNewAbility(abilityID, abilityLevel);
	newAbility->SetOwnerPawn(GetOwnerPawn());

	const ABILITY_ID_TYPE newAbilityID = newAbility->GetAbilityID();
	mapAbilities.emplace(newAbilityID, std::move(newAbility));

	return newAbilityID;
}

void AbilitySystemComponent::ActivateAbility(ABILITY_ID_TYPE abilityID)
{
	AbilityObject* grantAbility = GetAbility<AbilityObject>(abilityID);
	if (!grantAbility)
	{
		return;
	}

	grantAbility->ActivateAbility();
}

void AbilitySystemComponent::AllAbilitiesTriggerOff()
{
	for (auto& iterMapAbility : mapAbilities)
	{
		AbilityObject* ability = iterMapAbility.second.get();
		if (!ability)
		{
			continue;
		}

		ability->TriggerOff();
	}
}

std::shared_ptr<Pawn> AbilitySystemComponent::GetOwnerPawn() const
{
	return Cast<Pawn>(GetOwner());
}

#include "AbilitySystemComponent.h"
#include "Actor/Pawn/Pawn.h"

AbilitySystemComponent::AbilitySystemComponent()
{

}

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

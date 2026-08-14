#include "AbilitySystemComponent.h"
#include "Actor/Pawn/Pawn.h"

AbilitySystemComponent::AbilitySystemComponent()
{

}

void AbilitySystemComponent::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	for (auto& iterMapAbility : mapAbilities)
	{
		AbilityObject* ability = iterMapAbility.second.get();
		if (!ability)
		{
			continue;
		}

		ability->Tick(deltaTime);
	}
}

std::shared_ptr<Pawn> AbilitySystemComponent::GetOwnerPawn() const
{
	return Cast<Pawn>(GetOwner());
}

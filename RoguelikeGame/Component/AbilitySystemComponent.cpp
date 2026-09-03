#include "AbilitySystemComponent.h"
#include "Actor/Pawn/Pawn.h"

AbilitySystemComponent::AbilitySystemComponent()
{

}

AbilitySystemComponent::~AbilitySystemComponent() = default;


//폐기 예정
//void AbilitySystemComponent::PostTick(float deltaTime)
//{
//	super::PostTick(deltaTime);
//
//	for (auto& iterMapAbility : mapAbilities)
//	{
//		AbilityObject* ability = iterMapAbility.second.get();
//		if (!ability || !ability->IsTrigger())
//		{
//			continue;
//		}
//
//		ability->Tick(deltaTime);
//	}
//}

//void AbilitySystemComponent::Draw()
//{
//	super::Draw();
//
//	for (auto& iterMapAbility : mapAbilities)
//	{
//		AbilityObject* ability = iterMapAbility.second.get();
//		if (!ability || !ability->IsTrigger())
//		{
//			continue;
//		}
//
//		ability->Draw();
//	}
//}

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

void AbilitySystemComponent::Draw()
{
	super::Draw();

	for (auto& iterMapAbility : mapAbilities)
	{
		AbilityObject* ability = iterMapAbility.second.get();
		if (!ability)
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
	newAbility->SetCooldownChangeCallback(std::bind(&AbilitySystemComponent::OnAbilityCooldownStateChange, this, std::placeholders::_1, std::placeholders::_2));
	newAbility->SetActivateCallback(std::bind(&AbilitySystemComponent::OnAbilityActivate, this, std::placeholders::_1, std::placeholders::_2));

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

	/* 활성화 가능한지 여부 확인 */
	if (!grantAbility->CanActivateAbility())
	{
		return;
	}

	/* Ability 활성화 전 자원 소모 */
	grantAbility->ConsumeCost();

	/* Ability 활성화 */
	grantAbility->ActivateAbility();
}

void AbilitySystemComponent::SetAbilityCooldownChangeCallback(AbilityCooldownChangeCallback callback)
{
	onAbilityCooldownChange = callback;
}

void AbilitySystemComponent::SetAbilityActivateCallback(AbilityActivateCallback callback)
{
	onAbilityActivate = callback;
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

void AbilitySystemComponent::OnAbilityCooldownStateChange(const AbilityObject& ability, bool bCooldown)
{
	if (onAbilityCooldownChange)
	{
		onAbilityCooldownChange(ability, bCooldown);
	}
}

void AbilitySystemComponent::OnAbilityActivate(const AbilityObject& ability, bool bActivate)
{
	if (onAbilityActivate)
	{
		onAbilityActivate(ability, bActivate);
	}
}

std::shared_ptr<Pawn> AbilitySystemComponent::GetOwnerPawn() const
{
	return Cast<Pawn>(GetOwner());
}

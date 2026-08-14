#pragma once

#include <Component/Component.h>
#include <unordered_map>
#include "Ability/AbilityObject.h"


/* Ability들을 관리하는 컴포넌트 */
class AbilityObject;
class Pawn;
class AbilitySystemComponent : public Craft::Component
{
	TYPE_DECLARATIONS(AbilitySystemComponent, Component)

public:
	AbilitySystemComponent();
	virtual ~AbilitySystemComponent() = default;

public:
	virtual void Tick(float deltaTime) override;

public:
	void AbilityTriggerOn();
	void AbilityTriggerOff();

public:
	template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<AbilityObject, T>::value>>
	AbilityObject::ABILITY_ID_TYPE AddNewAbility(Args&& ...args)
	{
		std::unique_ptr<T> newAbility = std::make_unique<T>(std::forward<Args>(args)...);
		newAbility->SetOwnerPawn(GetOwnerPawn());
		const AbilityObject::ABILITY_ID_TYPE newAbilityID = newAbility->GetAbilityID();

		mapAbilities.emplace(newAbilityID, std::move(newAbility));

		return newAbilityID;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<AbilityObject, T>::value>>
	T* GetAbility(const AbilityObject::ABILITY_ID_TYPE abilityID)
	{
		auto iterFindAbility = mapAbilities.find(abilityID);
		if (iterFindAbility == mapAbilities.end())
		{
			return nullptr;
		}

		return Cast<T>(iterFindAbility->second);
	}

private:
	std::shared_ptr<Pawn> GetOwnerPawn() const;

private:
	/* 보유중인 Ability List*/
	std::unordered_map<AbilityObject::ABILITY_ID_TYPE, std::unique_ptr<AbilityObject>> mapAbilities;
};


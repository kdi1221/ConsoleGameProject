#pragma once

#include <Component/Component.h>
#include <unordered_map>
#include "Ability/AbilityObject.h"

class Pawn;
class AbilityObject;

/* Ability들을 관리하는 컴포넌트 */
class AbilitySystemComponent : public Craft::Component
{
	TYPE_DECLARATIONS(AbilitySystemComponent, Component)

public:
	AbilitySystemComponent();
	virtual ~AbilitySystemComponent();

public:
	//virtual void PostTick(float deltaTime) override;

	/* Ability 별 Tick */
	virtual void Tick(float deltaTime) override;

	/* Ability 별 상태 Draw */
	virtual void Draw() override;

public:
	/* 새로운 Ability 추가 */
	ABILITY_ID_TYPE AddNewAbility(ABILITY_ID_TYPE abilityID, int abilityLevel);

	/* Ability 활성화 */
	void ActivateAbility(ABILITY_ID_TYPE abilityID);


	template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<AbilityObject, T>::value>>
	ABILITY_ID_TYPE AddNewAbility(Args&& ...args)
	{
		std::unique_ptr<T> newAbility = std::make_unique<T>(std::forward<Args>(args)...);
		newAbility->SetOwnerPawn(GetOwnerPawn());
		const ABILITY_ID_TYPE newAbilityID = newAbility->GetAbilityID();

		mapAbilities.emplace(newAbilityID, std::move(newAbility));

		return newAbilityID;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<AbilityObject, T>::value>>
	T* GetAbility(const ABILITY_ID_TYPE abilityID)
	{
		auto iterFindAbility = mapAbilities.find(abilityID);
		if (iterFindAbility == mapAbilities.end())
		{
			return nullptr;
		}

		return Cast<T>(iterFindAbility->second);
	}

public:
	void AllAbilitiesTriggerOff();

private:
	/* Owner Pawn 참조 반환 */
	std::shared_ptr<Pawn> GetOwnerPawn() const;

private:
	/* 보유중인 Ability List*/
	std::unordered_map<ABILITY_ID_TYPE, std::unique_ptr<AbilityObject>> mapAbilities;
};


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

private:
	/* 보유한 Ability의 쿨다운 상태 변경에 따른 이벤트 타입 */
	using AbilityCooldownChangeCallback = std::function<void(const AbilityObject&, bool)>;

	/* 보유한 Ability의 Activate/End 시 호출되는 이벤트 */
	using AbilityActivateCallback = std::function<void(const AbilityObject&, bool)>;

public:
	AbilitySystemComponent();
	virtual ~AbilitySystemComponent();

public:
	/* Ability 별 Tick */
	virtual void Tick(float deltaTime) override;

	/* Ability 별 상태 Draw */
	virtual void Draw() override;

public:
	/* 새로운 Ability 추가 */
	ABILITY_ID_TYPE AddNewAbility(ABILITY_ID_TYPE abilityID, int abilityLevel);

	/* Ability 활성화 */
	void ActivateAbility(ABILITY_ID_TYPE abilityID);

	/* Ability 쿨다운 상태 변경시 호출되는 콜백 지정 */
	void SetAbilityCooldownChangeCallback(AbilityCooldownChangeCallback callback);

	/* Ability Activate / End 이벤트 콜백 지정 */
	void SetAbilityActivateCallback(AbilityActivateCallback callback);

	/* 모든 Ability 활성 중지 */
	void CancelAllAbility();

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

private:
	/* 보유한 Ability의 Cooldown 상태 변경시 호출 */
	void OnAbilityCooldownStateChange(const AbilityObject& ability, bool bCooldown);

	/* 보유한 Ability의 Activate,End 이벤트시 호출 */
	void OnAbilityActivate(const AbilityObject& ability, bool bActivate);

private:
	/* Owner Pawn 참조 반환 */
	std::shared_ptr<Pawn> GetOwnerPawn() const;

private:
	/* 보유중인 Ability의 Cooldown 상태 변경시 호출되는 콜백 */
	AbilityCooldownChangeCallback onAbilityCooldownChange;

	/* 보유중인 Ability의 Activate 상태 변경시 호출되는 콜백 */
	AbilityActivateCallback onAbilityActivate;

	/* 보유중인 Ability List*/
	std::unordered_map<ABILITY_ID_TYPE, std::unique_ptr<AbilityObject>> mapAbilities;
};


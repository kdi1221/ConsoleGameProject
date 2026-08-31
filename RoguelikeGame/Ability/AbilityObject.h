#pragma once

#include "Types/Defines.h"
#include <Util/Timer.h>
#include <Core/Core.h>
#include <Core/CraftObject.h>
#include <functional>

class Pawn;

/* Ability Base 클래스 */
class AbilityObject : public Craft::CraftObject
{
	TYPE_DECLARATIONS(AbilityObject, CraftObject)

private:
	/* 쿨다운 상태 변경에 따른 이벤트 타입 */
	using AbilityCooldownChangeCallback = std::function<void(const AbilityObject&, bool)>;

public:
	/* Ability ID에 대한 AbilityObject생성*/
	static std::unique_ptr<AbilityObject> CreateNewAbility(const ABILITY_ID_TYPE abilityID, int abilityLevel);

public:
	AbilityObject(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityObject();

public:
	virtual void Tick(float deltaTime);

	virtual void Draw();

	/* Ability 활성화 가능 여부 */
	virtual bool CanActivateAbility() const;

	/* Ability 사용에 필요한 자원 소모 */
	virtual void ConsumeCost();

	/* Ability 활성화 */
	virtual void ActivateAbility();

	/* Ability 종료 */
	virtual void EndAbility(bool bCancelAbility);

	/* Ability 취소 */
	virtual void CancelAbility();





	//--------------이 아래로 폐기 예정 ------------------//
	/* Ability 트리거 켜기 */
	virtual void TriggerOn();

	/* Ability 트리거 끄기 */
	virtual void TriggerOff();
	//--------------이 위로 폐기 예정 ------------------//




public:
	/* Ability의 소유 폰 지정 */
	void SetOwnerPawn(std::weak_ptr<Pawn> newOwner);

	/* Ability 레벨 지정 */
	void SetAbilityLevel(int level);
	
	/* 쿨다운 타임 지정 */
	void SetCooldownTime(float delayTime);

	/* 마나 코스트 지정 */
	void SetManaCost(float amount);

	/* 쿨다운 상태 변경에 대한 이벤트 콜백 지정 */
	void SetCooldownChangeCallback(AbilityCooldownChangeCallback callback);

public:
	inline ABILITY_ID_TYPE GetAbilityID() const { return abilityID; }
	inline int GetAbilityLevel() const { return abilityLevel; }
	inline bool IsActivate() const { return bActivated; }
	inline bool IsCooldown() const { return bCooldownActive; }
	inline float GetCooldownTime() const { return cooldownTime; }


	inline bool IsTrigger() const { return bTrigger; }

private:
	/* 쿨다운 활성화 */
	void ActivateCooldown();

	/* 쿨다운 초기화 */
	void ResetCooldown();

protected:
	std::shared_ptr<Pawn> GetOwnerPawn() const { return ownerPawn.lock(); }

private:
	//Ability의 고유 ID
	ABILITY_ID_TYPE abilityID = 0;

	//Ability Level
	int abilityLevel = 1;

	//Ability 활성화 여부
	bool bActivated = false;
	
	//Ability Cooldown Time;
	float cooldownTime = 0.f;

	//Cooldown 지정 여부
	bool bCooldownActive = false;

	//Cooldown Timer
	Timer timerCooldown;

	//Cooldown 상태 변경시 호출되는 콜백
	AbilityCooldownChangeCallback onCooldownChange;

	/* Ability 사용에 필요한 Mana(플레이어 전용) */
	float costManaValue = 0.f;


	// Ability Trigger 여부(폐기 예정)
	bool bTrigger = false;

	// 이 Ability를 소유중인 Pawn
	std::weak_ptr<Pawn> ownerPawn;
};


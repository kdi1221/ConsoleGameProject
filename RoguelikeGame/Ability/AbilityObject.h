#pragma once

#include "Types/Defines.h"
#include <Core/Core.h>
#include <Core/CraftObject.h>

class Pawn;

/* Ability Base 클래스 */
class AbilityObject : public Craft::CraftObject
{
	TYPE_DECLARATIONS(AbilityObject, CraftObject)

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
	void SetOwnerPawn(std::weak_ptr<Pawn> newOwner);
	void SetAbilityLevel(int level);

public:
	inline ABILITY_ID_TYPE GetAbilityID() const { return abilityID; }
	inline int GetAbilityLevel() const { return abilityLevel; }
	inline bool IsTrigger() const { return bTrigger; }

protected:
	std::shared_ptr<Pawn> GetOwnerPawn() const { return ownerPawn.lock(); }

private:
	//Ability의 고유 ID
	ABILITY_ID_TYPE abilityID = 0;

	//Ability Level
	int abilityLevel = 1;

	// Ability Trigger 여부
	bool bTrigger = false;

	// 이 Ability를 소유중인 Pawn
	std::weak_ptr<Pawn> ownerPawn;
};


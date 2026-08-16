#pragma once

#include <Core/Core.h>
#include <Core/CraftObject.h>

class Pawn;

/* Ability Base 클래스 */
class AbilityObject : public Craft::CraftObject
{
	TYPE_DECLARATIONS(AbilityObject, CraftObject)

public:
	using ABILITY_ID_TYPE = int;
	static constexpr ABILITY_ID_TYPE INVALID_ABILITY_ID = 0;

public:
	AbilityObject(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityObject();

public:
	virtual void Tick(float deltaTime) = 0;

	virtual void Draw();

	/* Ability 트리거 켜기 */
	virtual void TriggerOn();

	/* Ability 트리거 끄기 */
	virtual void TriggerOff();

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


#pragma once

#include <Core/Core.h>
#include <Core/CraftObject.h>

class Pawn;

/* Ability Base 클래스 */
class AbilityObject : Craft::CraftObject
{
	TYPE_DECLARATIONS(AbilityObject, CraftObject)

public:
	using ABILITY_ID_TYPE = unsigned int;
	static constexpr ABILITY_ID_TYPE INVALID_ABILITY_ID = 0;

public:
	AbilityObject();
	virtual ~AbilityObject();

public:
	virtual void Tick(float deltaTime) = 0;

	/* Ability 트리거 켜기 */
	virtual void TriggerOn() = 0;

	/* Ability 트리거 끄기 */
	virtual void TriggerOff() = 0;

public:
	void SetOwnerPawn(std::weak_ptr<Pawn> newOwner);

public:
	ABILITY_ID_TYPE GetAbilityID() const { return abilityID; }

protected:
	std::shared_ptr<Pawn> GetOwnerPawn() const { return ownerPawn.lock(); }

private:
	//Ability의 고유 ID
	ABILITY_ID_TYPE abilityID = 0;

	// 이 Ability를 소유중인 Pawn
	std::weak_ptr<Pawn> ownerPawn;
};


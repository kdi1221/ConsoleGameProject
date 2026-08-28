#include "AbilityObject.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include <Math/Vector2Float.h>


//Ability 객체 생성을 위한 헤더파일 포함
#include "PlayerAbility/AbilityShotSpiritBall.h"
#include "PlayerAbility/AbilityFrozenOrb.h"

std::unique_ptr<AbilityObject> AbilityObject::CreateNewAbility(const ABILITY_ID_TYPE abilityID, int abilityLevel)
{
	//TODO : abilityID에 따른 생성 Ability 구분 필요..
	switch (abilityID)
	{
	case 1:
		return std::make_unique<AbilityShotSpiritBall>(abilityID, abilityLevel);

	case 2:
		return std::make_unique<AbilityFrozenOrb>(abilityID, abilityLevel);
	}

	return nullptr;
}


AbilityObject::AbilityObject(ABILITY_ID_TYPE id, int level)
	:abilityID(id)
	,abilityLevel(level)
{

}

AbilityObject::~AbilityObject()
{

}

void AbilityObject::Tick(float deltaTime)
{
	/* 쿨다운 진행 처리 */
	if (bCooldownActive)
	{
		timerCooldown.Tick(deltaTime);
		if (timerCooldown.IsTimeOut())
		{
			ResetCooldown();
		}
	}
	
}

void AbilityObject::Draw()
{

}

bool AbilityObject::CanActivateAbility() const
{
	/* 쿨다운 상태에서는 사용 못함 */
	if (bCooldownActive)
	{
		return false;
	}

	/* 플레이어 전용 : 소모되는 마나가 있을때는 마나가 남아있는지 체크 해야 함 */
	if (costManaValue > 0.f)
	{
		std::shared_ptr<PlayerPawn> ownerPlayerPawn = Cast<PlayerPawn>(GetOwnerPawn());
		if (ownerPlayerPawn && ownerPlayerPawn->GetCurrentMana() < costManaValue)
		{
			return false;
		}
	}

	return true;
}

void AbilityObject::ConsumeCost()
{
	/* 플레이어 전용 : 소모되는 마나가 있을때는 플레이어의 마나 소모 */
	if (costManaValue > 0.f)
	{
		if (std::shared_ptr<PlayerPawn> ownerPlayerPawn = Cast<PlayerPawn>(GetOwnerPawn()))
		{
			ownerPlayerPawn->ConsumeMana(costManaValue);
		}
	}
}

void AbilityObject::ActivateAbility()
{
	bActivated = true;
}

void AbilityObject::EndAbility(bool bCancelAbility)
{
	bActivated = false;

	ActivateCooldown();
}

void AbilityObject::CancelAbility()
{
	EndAbility(true);
}





void AbilityObject::TriggerOn()
{
	bTrigger = true;
}

void AbilityObject::TriggerOff()
{
	bTrigger = false;
}




void AbilityObject::SetOwnerPawn(std::weak_ptr<Pawn> newOwner)
{
	ownerPawn = newOwner;
}

void AbilityObject::SetAbilityLevel(int level)
{
	abilityLevel = level;
}

void AbilityObject::SetCooldownTime(float delayTime)
{
	cooldownTime = delayTime;
}

void AbilityObject::SetManaCost(float amount)
{
	costManaValue = amount;
}

void AbilityObject::SetCooldownChangeCallback(AbilityCooldownChangeCallback callback)
{
	onCooldownChange = callback;
}

void AbilityObject::ActivateCooldown()
{
	/* 쿨다운 타임이 지정되어있지않으면 쿨다운 활성화 x */
	if (cooldownTime <= Craft::EPSILON)
	{
		return;
	}

	timerCooldown.Reset();
	timerCooldown.SetTargetTime(cooldownTime);
	bCooldownActive = true;

	/* 쿨다운 설정되었으니 콜백 호출 */
	if (onCooldownChange)
	{
		onCooldownChange(*this, true);
	}
}

void AbilityObject::ResetCooldown()
{
	timerCooldown.Reset();
	bCooldownActive = false;

	/* 쿨다운 해제 되었으니 콜백 호출 */
	if (onCooldownChange)
	{
		onCooldownChange(*this, false);
	}
}

#pragma once

#include "Actor/Pawn/Pawn.h"
#include "Component/InputComponent.h"
#include "Component/MovementComponent.h"
#include <unordered_set>

namespace Craft
{
	class CameraComponent;
}

class FieldSkillItem;

class PlayerPawn : public Pawn
{
	TYPE_DECLARATIONS(PlayerPawn, Pawn)

public:
	using OnItemGainEventType = std::function<void(int)>;

public:
	PlayerPawn(const Craft::Vector2Int& position);
	~PlayerPawn() = default;

private:
	virtual void BeginPlay() override;
	virtual void PreTick(float deltaTime) override;
	virtual void Tick(float deltaTime) override;
	virtual void OnUpdatedPosition(const Craft::Vector2Int& prevLocalPosition,
									const Craft::Vector2Int& prevWorldPosition,
									const Craft::Vector2Int& localPosition,
									const Craft::Vector2Int& worldPosition) override;

public:
	/* 플레이어가 특정 스킬 아이템 획득 */
	void GainSkillItem(std::shared_ptr<FieldSkillItem> gainItem);

	/* 플레이어가 특정 스킬 아이템 획득시 호출되는 이벤트 콜백 설정 */
	void SetOnItemGainEvent(OnItemGainEventType callback);

	/* 플레이어에 특정 스킬 부여 */
	void GrantAbility(int abilityID, int abilityLevel);

private:
	/* 키 입력 콜백 */
	void OnMoveKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);
	void OnProjectileFireKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);
	void OnCheatInputTrigger(int keyCode, Craft::eInputTrigger inputTrigger);

private:
	/* 위치 이동 시 호출되는 카메라 뷰 위치 변경 함수 */
	void UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition);

	/* 발사 입력 처리 */
	void ProcessFireInput();

	/* 이동 입력 처리 */
	void ProcessMoveInput();

	/* Projectile Ability의 트리거 ON/OFF*/
	void SetProjectileAbilityTrigger(bool bTrigger);

private:
	/* 현재 프레임에서의 누적 이동 입력 값*/
	//Craft::Vector2Int moveInputValue = Craft::Vector2Int::Zero;

	/* 현재 프레임에서의 이동 방향 입력 값*/
	//Craft::Vector2Float moveInputValue = Craft::Vector2Float::Zero;

	/* 현재 프레임에서의 이동 입력 방향 */
	Craft::eDirection moveInputDirection = Craft::eDirection::None;

	/* 현재 프레임에서의 공격 입력 누적 값*/
	Craft::Vector2Int fireInputValue = Craft::Vector2Int::Zero;

	/* 이전 프레임에서의 공격 입력 누적 값*/
	Craft::Vector2Int prevFireInputValue = Craft::Vector2Int::Zero;
	
	/* 부여된 Projectile Ability ID들 */
	std::unordered_set<AbilityObject::ABILITY_ID_TYPE> grantProjectileAbilities;

private:
	OnItemGainEventType onItemGainEvent;

private:
	/* 플레이어 폰을 바라보는 카메라 컴포넌트 */
	std::shared_ptr<Craft::CameraComponent> cameraComponent;

	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::MovementComponent> movementComponent;

	/* 입력 컴포넌트 */
	std::shared_ptr<Craft::InputComponent> inputComponent;
};


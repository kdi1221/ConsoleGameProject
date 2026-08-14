#pragma once

#include "Actor/Pawn/Pawn.h"
#include "Component/InputComponent.h"
#include "Component/MovementComponent.h"

namespace Craft
{
	class CameraComponent;
}

class PlayerPawn : public Pawn
{
	TYPE_DECLARATIONS(PlayerPawn, Pawn)

public:
	PlayerPawn(const Craft::Vector2Int& position);
	~PlayerPawn() = default;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnUpdatedPosition(const Craft::Vector2Int& prevLocalPosition,
									const Craft::Vector2Int& prevWorldPosition,
									const Craft::Vector2Int& localPosition,
									const Craft::Vector2Int& worldPosition) override;

	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility() override;

private:
	/* 키 입력 콜백 */
	void OnMoveKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);
	void OnProjectileFireKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);

private:
	/* 위치 이동 시 호출되는 카메라 뷰 위치 변경 함수 */
	void UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition);

	/* 발사 입력 처리 */
	void ProcessFireInput();

	/* 이동 입력 처리 */
	void ProcessMoveInput();

	/* Projectile 발사 Offset 지정 */
	void SetProjectileSpawnOffset(const Craft::Vector2Int& spawnOffset);

	/* Projectile 목표 지점 설정 */
	void SetAimingPostion(const Craft::Vector2Int& position);

	/* Projectile Ability의 트리거 ON/OFF*/
	void SetProjectileAbilityTrigger(bool bTrigger);

private:
	/* 현재 프레임에서의 누적 이동 입력 값*/
	Craft::Vector2Int moveInputValue = Craft::Vector2Int::Zero;

	/* 현재 프레임에서의 공격 입력 누적 값*/
	Craft::Vector2Int fireInputValue = Craft::Vector2Int::Zero;

	/* 이전 프레임에서의 공격 입력 누적 값*/
	Craft::Vector2Int prevFireInputValue = Craft::Vector2Int::Zero;

	/* Projectile 발사 시 최대 Range */
	float ProjectileRange = 10.f;
	
	/* 부여된 Projectile Ability ID들 */
	std::vector<AbilityObject::ABILITY_ID_TYPE> grantProjectileAbilities;

private:
	/* 플레이어 폰을 바라보는 카메라 컴포넌트 */
	std::shared_ptr<Craft::CameraComponent> cameraComponent;

	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::MovementComponent> movementComponent;
};


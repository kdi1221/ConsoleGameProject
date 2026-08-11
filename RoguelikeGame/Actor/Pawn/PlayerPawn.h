#pragma once

#include "Pawn.h"
#include "Component/InputComponent.h"

namespace Craft
{
	class CameraComponent;
}

class FireProjectileComponent;

class PlayerPawn : public Pawn
{
	TYPE_DECLARATIONS(PlayerPawn, Pawn)

public:
	PlayerPawn(const Craft::Vector2Float& position);
	~PlayerPawn() = default;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnUpdatedPosition(const Craft::Vector2Float& prevLocalPosition,
									const Craft::Vector2Float& prevWorldPosition,
									const Craft::Vector2Float& localPosition,
									const Craft::Vector2Float& worldPosition) override;

private:
	void OnMoveKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);
	void OnProjectileFireKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);
	void UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition);
	void ProcessFireInput();

private:
	/* 현재 프레임에서의 누적 이동 입력 값*/
	Craft::Vector2Int moveInputValue = Craft::Vector2Int::Zero;

	/* 현재 프레임에서의 공격 입력 누적 값*/
	Craft::Vector2Int fireInputValue = Craft::Vector2Int::Zero;

	/* 이전 프레임에서의 공격 입력 누적 값*/
	Craft::Vector2Int prevFireInputValue = Craft::Vector2Int::Zero;

	/* Projectile 발사 시 최대 Range */
	float ProjectileRange = 10.f;

private:
	/* 플레이어 폰을 바라보는 카메라 컴포넌트 */
	std::shared_ptr<Craft::CameraComponent> cameraComponent;

	/* 탄환 발사 컴포넌트 */
	std::shared_ptr<FireProjectileComponent> fireProjectileComponent;
};


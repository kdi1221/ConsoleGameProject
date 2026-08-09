#pragma once

#include "Pawn.h"

namespace Craft
{
	class CameraComponent;
}

class PlayerPawn : public Pawn
{
	TYPE_DECLARATIONS(PlayerPawn, Pawn)

public:
	PlayerPawn(const Craft::Vector2Float& position);
	~PlayerPawn() = default;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:
	void OnMoveKeyInput(int keyCode);
	void OnUpdatePosition(const Craft::Vector2Float & localPosition, const Craft::Vector2Float& worldPosition);
	void UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition);

private:
	/* 현재 프레임에서의 누적 이동 입력 값*/
	Craft::Vector2Int moveInputValue = Craft::Vector2Int::Zero;

	/* 플레이어 폰을 바라보는 카메라 컴포넌트 */
	std::shared_ptr<Craft::CameraComponent> cameraComponent;
};


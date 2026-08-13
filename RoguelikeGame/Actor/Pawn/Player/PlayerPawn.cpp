#include "PlayerPawn.h"
#include "Math/Color.h"
#include "Component/CameraComponent.h"
#include "Component/FireProjectileComponent.h"
#include "Actor/Projectile/Projectile.h"
#include <Math/Vector2Float.h>
#include <cassert>
#include <Windows.h>

using namespace Craft;

PlayerPawn::PlayerPawn(const Craft::Vector2Int& position)
	:super(position, L"☺", Color::Yellow, 5, eTeamID::Player)
{
	std::shared_ptr<InputComponent> inputComponent = AddComponent<InputComponent>();
	assert(inputComponent && "inputComponent create fail..");

	/* 이동 입력 트리거 */
	InputComponent::FInputTrigger moveInputTrigger(eInputTrigger::Press,
												std::bind(&PlayerPawn::OnMoveKeyInput, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback('W', moveInputTrigger);
	inputComponent->AddInputCallback('S', moveInputTrigger);
	inputComponent->AddInputCallback('A', moveInputTrigger);
	inputComponent->AddInputCallback('D', moveInputTrigger);

	/* 발사 입력 트리거 */
	InputComponent::FInputTrigger fireInputTrigger(eInputTrigger::Up | eInputTrigger::Down,
												std::bind(&PlayerPawn::OnProjectileFireKeyInput, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback(VK_UP, fireInputTrigger);
	inputComponent->AddInputCallback(VK_DOWN, fireInputTrigger);
	inputComponent->AddInputCallback(VK_LEFT, fireInputTrigger);
	inputComponent->AddInputCallback(VK_RIGHT, fireInputTrigger);

	/* 카메라 컴포넌트 */
	cameraComponent = AddComponent<CameraComponent>();
	assert(cameraComponent && "cameraComponent create fail..");

	/* 투사체 발사 컴포넌트 */
	fireProjectileComponent = AddComponent<FireProjectileComponent>(0.5f, L"*", Color::Green, 0.04f, 0.06f, GetTeamID(), 1);
	assert(fireProjectileComponent && "fireProjectileComponent create fail..");

	/* 이동 컴포넌트 */
	movementComponent = AddComponent<MovementComponent>(0.05f);
	assert(movementComponent && "cameraComponent create fail..");
}

void PlayerPawn::BeginPlay()
{
	super::BeginPlay();

	UpdateViewCameraPosition(GetWorldPosition());
}

void PlayerPawn::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	/* 이동 입력 처리 */
	ProcessMoveInput();


	/* fire 입력 처리 */
	ProcessFireInput();
}

void PlayerPawn::OnUpdatedPosition(const Craft::Vector2Int& prevLocalPosition,
									const Craft::Vector2Int& prevWorldPosition,
									const Craft::Vector2Int& localPosition,
									const Craft::Vector2Int& worldPosition)
{
	super::OnUpdatedPosition(prevLocalPosition, prevWorldPosition, localPosition, worldPosition);

	/* 위치 업데이트 시 카메라의 View Position도 업데이트 한다. */
	UpdateViewCameraPosition(worldPosition);
}

void PlayerPawn::OnMoveKeyInput(int keyCode, eInputTrigger inputTrigger)
{
	switch (keyCode)
	{
	case 'W':
		{
			moveInputValue += Vector2Int::Up;
		}
		break;

	case 'S':
		{
			moveInputValue += Vector2Int::Down;
		}
		break;

	case 'A':
		{
			moveInputValue += Vector2Int::Left;
		}
		break;

	case 'D':
		{
			moveInputValue += Vector2Int::Right;
		}
		break;
	}
}

void PlayerPawn::OnProjectileFireKeyInput(int keyCode, eInputTrigger inputTrigger)
{
	Vector2Int addInputValue = Vector2Int::Zero;
	switch (keyCode)
	{
	case VK_UP:
		{
			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Up : Vector2Int::Down;
		}
		break;

	case VK_DOWN:
		{
			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Down : Vector2Int::Up;
		}
		break;

	case VK_LEFT:
		{
			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Left : Vector2Int::Right;
		}
		break;

	case VK_RIGHT:
		{
			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Right : Vector2Int::Left;
		}
		break;
	}

	fireInputValue += addInputValue;
}

void PlayerPawn::UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition)
{
	assert(cameraComponent && "cameraComponent create fail..");
	cameraComponent->SetViewPosition(viewPosition);
}

void PlayerPawn::ProcessFireInput()
{
	assert(fireProjectileComponent && "Invalid fireProjectileComponent");

	if (prevFireInputValue != fireInputValue)
	{
		if (fireInputValue == Vector2Int::Zero)
		{
			/* 이전에 발사중이었다가 중지 됨 */
			fireProjectileComponent->SetEnableFire(false);
		}
		else if (prevFireInputValue == Vector2Int::Zero)
		{
			/* 발사 중지 상태에서 발사 상태로 전환 */
			fireProjectileComponent->SetEnableFire(true);
		}

		prevFireInputValue = fireInputValue;
	}

	/* Projectile이 생성될 Offset 지정 */
	if (fireInputValue != Vector2Int::Zero)
	{
		/* 생성될 Projectile이 생성될 Offset 지정 */
		const Vector2Int& spawnOffset = fireInputValue;
		fireProjectileComponent->SetProjectileSpawnOffset(spawnOffset);

		/* 생성될 Projectile의 목표 위치 지정(offset 위치로 향하는 방향 * Range) */
		Vector2Float fireDirection = static_cast<Vector2Float>(spawnOffset);
		fireDirection.Normalize();
		const Vector2Float aimingPosition = static_cast<Vector2Float>(GetWorldPosition()) + (fireDirection * ProjectileRange);
		fireProjectileComponent->SetProjectileAimingPosition(Vector2Int(static_cast<int>(round(aimingPosition.x)), static_cast<int>(round(aimingPosition.y))));
	}
}

void PlayerPawn::ProcessMoveInput()
{
	assert(movementComponent && "Invalid movementComponent");

	movementComponent->SetLastMoveDirection(moveInputValue);
	moveInputValue = Vector2Int::Zero;
}

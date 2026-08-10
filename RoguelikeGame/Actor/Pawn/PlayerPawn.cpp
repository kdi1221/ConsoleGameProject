#include "PlayerPawn.h"
#include "Math/Color.h"
#include "Component/TransformComponent.h"
#include "Component/CameraComponent.h"
#include "Component/FireProjectileComponent.h"
#include "Actor/Projectile/Projectile.h"
#include <cassert>
#include <Windows.h>

using namespace Craft;

PlayerPawn::PlayerPawn(const Craft::Vector2Float& position)
	:super(position, L"☺", Color::Yellow, 1, 0.1f)
{
	std::shared_ptr<InputComponent> inputComponent = AddComponent<InputComponent>();
	assert(inputComponent && "inputComponent create fail..");

	InputComponent::FInputTrigger moveInputTrigger(eInputTrigger::Press,
												std::bind(&PlayerPawn::OnMoveKeyInput, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback('W', moveInputTrigger);
	inputComponent->AddInputCallback('S', moveInputTrigger);
	inputComponent->AddInputCallback('A', moveInputTrigger);
	inputComponent->AddInputCallback('D', moveInputTrigger);

	InputComponent::FInputTrigger fireInputTrigger(eInputTrigger::Up | eInputTrigger::Down,
												std::bind(&PlayerPawn::OnProjectileFireKeyInput, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback(VK_UP, fireInputTrigger);
	inputComponent->AddInputCallback(VK_DOWN, fireInputTrigger);
	inputComponent->AddInputCallback(VK_LEFT, fireInputTrigger);
	inputComponent->AddInputCallback(VK_RIGHT, fireInputTrigger);

	std::shared_ptr<TransformComponent> transformComponent = GetTransform();
	assert(transformComponent && "transformComponent invalid..");
	transformComponent->SetUpdatedPositionCallback(std::bind(&PlayerPawn::OnUpdatePosition, this, std::placeholders::_1, std::placeholders::_2));

	cameraComponent = AddComponent<CameraComponent>();
	assert(cameraComponent && "cameraComponent create fail..");

	fireProjectileComponent = AddComponent<FireProjectileComponent>(0.5f, L"*", Color::Green, 0.03f);
	assert(fireProjectileComponent && "fireProjectileComponent create fail..");
}

void PlayerPawn::BeginPlay()
{
	super::BeginPlay();

	UpdateViewCameraPosition(static_cast<Vector2Int>(GetWorldPosition()));
}

void PlayerPawn::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	/* 이동 입력을 movementComponent에 전달 */
	if (moveInputValue != Vector2Int::Zero)
	{
		SetLastMoveDirection(moveInputValue);
		moveInputValue = Vector2Int::Zero;
	}

	/* fire 입력 처리 */
	if (prevFireInputValue != fireInputValue)
	{
		if (fireInputValue == Vector2Int::Zero)
		{
			/* 이전에 발사중이었다가 중지 됨 */
			fireProjectileComponent->SetEnableFire(false);
		}
		else
		{
			//SetLookDirection(static_cast<Vector2Float>(fireInputValue));
			assert(fireProjectileComponent && "Invalid fireProjectileComponent");

			/* 생성될 Projectile이 생성될 Offset 지정 */
			const Vector2Float spawnOffset = static_cast<Vector2Float>(fireInputValue);
			fireProjectileComponent->SetProjectileSpawnOffset(spawnOffset);

			/* 생성될 Projectile의 목표 위치 지정(offset 위치로 향하는 방향 * Range) */
			Vector2Float fireDirection = spawnOffset;
			fireDirection.Normalize();
			const Vector2Float AimingPosition = GetWorldPosition() + (fireDirection * ProjectileRange);
			fireProjectileComponent->SetProjectileAimingPosition(AimingPosition);

			if (prevFireInputValue == Vector2Int::Zero)
			{
				/* 발사 중지 상태에서 발사 상태로 전환 */
				fireProjectileComponent->SetEnableFire(true);
			}
		}

		prevFireInputValue = fireInputValue;
	}
	
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

void PlayerPawn::OnUpdatePosition(const Vector2Float& localPosition, const Vector2Float& worldPosition)
{
	UpdateViewCameraPosition(static_cast<Vector2Int>(worldPosition));
}

void PlayerPawn::UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition)
{
	assert(cameraComponent && "cameraComponent create fail..");
	cameraComponent->SetViewPosition(viewPosition);
}

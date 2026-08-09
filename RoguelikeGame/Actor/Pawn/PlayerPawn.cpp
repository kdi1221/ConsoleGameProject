#include "PlayerPawn.h"
#include "Math/Color.h"
#include "Component/TransformComponent.h"
#include "Component/InputComponent.h"
#include "Component/CameraComponent.h"
#include <cassert>
#include <Windows.h>

using namespace Craft;

PlayerPawn::PlayerPawn(const Craft::Vector2Float& position)
	:super(position, "@", Color::Yellow, 1, 0.1f)
{
	std::shared_ptr<InputComponent> inputComponent = AddComponent<InputComponent>();
	assert(inputComponent && "inputComponent create fail..");

	InputComponent::FInputTrigger InputTrigger(InputComponent::eInputTrigger::Press,
												std::bind(&PlayerPawn::OnMoveKeyInput, this, std::placeholders::_1));

	inputComponent->AddInputCallback('W', InputTrigger);
	inputComponent->AddInputCallback('S', InputTrigger);
	inputComponent->AddInputCallback('A', InputTrigger);
	inputComponent->AddInputCallback('D', InputTrigger);

	std::shared_ptr<TransformComponent> transformComponent = GetTransform();
	assert(transformComponent && "transformComponent invalid..");
	transformComponent->SetUpdatedPositionCallback(std::bind(&PlayerPawn::OnUpdatePosition, this, std::placeholders::_1, std::placeholders::_2));

	cameraComponent = AddComponent<CameraComponent>();
	assert(cameraComponent && "cameraComponent create fail..");
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
}

void PlayerPawn::OnMoveKeyInput(int keyCode)
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

void PlayerPawn::OnUpdatePosition(const Vector2Float& localPosition, const Vector2Float& worldPosition)
{
	UpdateViewCameraPosition(static_cast<Vector2Int>(worldPosition));
}

void PlayerPawn::UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition)
{
	assert(cameraComponent && "cameraComponent create fail..");
	cameraComponent->SetViewPosition(viewPosition);
}

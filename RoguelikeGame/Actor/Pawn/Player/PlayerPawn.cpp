#include "PlayerPawn.h"
#include "Math/Color.h"
#include "Component/CameraComponent.h"
#include "Component/AbilitySystemComponent.h"
#include "Game/State/PlayerState/PlayerAbilityInfo.h"


//#include "Ability/AbilityBeam.h"


#include "Actor/FieldItem/FieldSkillItem.h"
#include "Item/ItemData/ItemDataTable.h"
#include "Game/State/GameMode/GM_Roguelike.h"
#include <Engine/Engine.h>
#include <Engine/Config/ConfigBase.h>
#include <Core/Input.h>
#include <Math/Vector2Float.h>
#include <cassert>
#include <Windows.h>

using namespace Craft;

using InputTriggerType = InputComponent::FInputTrigger;

PlayerPawn::PlayerPawn(const Craft::Vector2Int& position)
	:super(position, L"☺", Color::Yellow, 100.f, eTeamID::Player)
{
	inputComponent = AddComponent<InputComponent>();
	assert(inputComponent && "inputComponent create fail..");

	/* 이동 입력 트리거 */
	InputTriggerType moveInputTrigger(eInputTrigger::Press,
									std::bind(&PlayerPawn::OnMoveKeyInput, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback(VK_LBUTTON, moveInputTrigger);

	/* 치트 및 디버깅용 트리거 */
	InputComponent::FInputTrigger cheatInputTrigger(eInputTrigger::Down,
		std::bind(&PlayerPawn::OnCheatInputTrigger, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback('P', cheatInputTrigger);
	inputComponent->AddInputCallback('I', cheatInputTrigger);
	
	/* 이동 컴포넌트 */
	movementComponent = AddComponent<MovementComponent>(30.f);
	assert(movementComponent && "movementComponent create fail..");

	/* 카메라 컴포넌트 */
	cameraComponent = AddComponent<CameraComponent>();
	assert(cameraComponent && "cameraComponent create fail..");

	// 공격 범위 지정
	SetFireRange(10.f);
}

void PlayerPawn::BeginPlay()
{
	super::BeginPlay();

	UpdateViewCameraPosition(GetWorldPosition());
}

void PlayerPawn::PreTick(float deltaTime)
{
	super::PreTick(deltaTime);

	/* 조준 입력 처리 */
	ProcessAimingInput();

	/* 이동 입력 처리 */
	ProcessMoveInput();

	/* fire 입력 처리 */
	//ProcessFireInput();
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

void PlayerPawn::GainSkillItem(std::shared_ptr<FieldSkillItem> gainItem)
{
	assert(gainItem && "Invalid gainItem..");

	//const int gainItemID = gainItem->GetItemID();

	//const ItemData& gainItemData = ItemDataTable::GetItemData(gainItemID);
	//auto iterfindGrantAbility = grantProjectileAbilities.find(gainItemData.abilityID);
	//if (iterfindGrantAbility == grantProjectileAbilities.end())
	//{
	//	//획득한 스킬 아이템으로 Ability 추가
	//	GrantAbility(gainItemData.abilityID, 1);
	//}
	//else
	//{
	//	/* 기존 Ability 강화 */
	//	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	//	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	//	AbilityObject* grantedAbility = abilitySystemComponent->GetAbility<AbilityObject>(*iterfindGrantAbility);
	//	assert(grantedAbility && "Invalid grantedAbility");

	//	grantedAbility->SetAbilityLevel(min(gainItemData.maxNum, grantedAbility->GetAbilityLevel() + 1));
	//}

	///* 아이템 획득했음을 알림 */
	//if (onItemGainEvent)
	//{
	//	onItemGainEvent(gainItemID);
	//}
}

void PlayerPawn::SetOnItemGainEvent(OnItemGainEventType callback)
{
	onItemGainEvent = callback;
}

void PlayerPawn::GrantAbility(const PlayerAbilityInfo& abilityInfo)
{
	/* Ability 부여 */
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	ABILITY_ID_TYPE grantAbilityID = INVALID_ABILITY_ID;
	grantAbilityID = abilitySystemComponent->AddNewAbility(abilityInfo.GetAbilityID(), abilityInfo.GetAbilityLevel());

	/* Ability 입력 트리거 설정 */
	assert(inputComponent && "Invalid inputComponent");

	const int bindingKeyCode = abilityInfo.GetBindingKeyCode();
	InputTriggerType abilityActiveTrigger(eInputTrigger::Down,
										std::bind(&PlayerPawn::OnAbilityActiveKeyDown, this, std::placeholders::_1, std::placeholders::_2));

	inputComponent->AddInputCallback(bindingKeyCode, abilityActiveTrigger);

	/* KeyCode - grantAbilityID 별로 저장 */
	mapInputGrantAbilities.insert({ bindingKeyCode, grantAbilityID });
}

void PlayerPawn::OnMoveKeyInput(int keyCode, eInputTrigger inputTrigger)
{
	assert(inputComponent && "Invalid inputComponent");

	//마우스 커서의 월드상 위치로 향하는 방향과 가장일치하는 8방향 중의 하나를 이동방향으로 삼는다.
	eDirection newMoveDirection = eDirection::None;

	const Vector2Int& cursorWorldPos = inputComponent->GetLastMosueCursorPos();
	const Vector2Int& currentPos = GetWorldPosition();
	if (currentPos != cursorWorldPos)
	{
		Vector2Float toCursorPosDirection = static_cast<Vector2Float>(cursorWorldPos - currentPos);
		toCursorPosDirection.Normalize();

		float maxDot = -1.f;
		for (const auto& moveDirection : MOVE_DIRECTION)
		{
			const Vector2Float& currentDirection = moveDirection.second;
			float dotResult = currentDirection.DotProduct(toCursorPosDirection);

			if (dotResult > maxDot)
			{
				maxDot = dotResult;
				newMoveDirection = moveDirection.first;
			}
		}
	}
	else
	{
		newMoveDirection = eDirection::None;
	}

	moveInputDirection = newMoveDirection;
}

void PlayerPawn::OnAbilityActiveKeyDown(int keyCode, Craft::eInputTrigger inputTrigger)
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	const auto& iterFindGrantAbilityID = mapInputGrantAbilities.find(keyCode);
	if (iterFindGrantAbilityID == mapInputGrantAbilities.end())
	{
		return;
	}

	/* Input에 연결된 Ability ID에 대한 활성화 요청 */
	const ABILITY_ID_TYPE inputAbilityID = iterFindGrantAbilityID->second;
	abilitySystemComponent->ActivateAbility(inputAbilityID);
}

//void PlayerPawn::OnProjectileFireKeyInput(int keyCode, eInputTrigger inputTrigger)
//{
//	Vector2Int addInputValue = Vector2Int::Zero;
//	switch (keyCode)
//	{
//	case VK_UP:
//		{
//			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Up : Vector2Int::Down;
//		}
//		break;
//
//	case VK_DOWN:
//		{
//			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Down : Vector2Int::Up;
//		}
//		break;
//
//	case VK_LEFT:
//		{
//			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Left : Vector2Int::Right;
//		}
//		break;
//
//	case VK_RIGHT:
//		{
//			addInputValue = eInputTrigger::None != (inputTrigger & eInputTrigger::Down) ? Vector2Int::Right : Vector2Int::Left;
//		}
//		break;
//	}
//
//	fireInputValue += addInputValue;
//}

void PlayerPawn::OnCheatInputTrigger(int keyCode, Craft::eInputTrigger inputTrigger)
{
	Engine& engine = Engine::Get();

	switch (keyCode)
	{
	case 'P':
		{
			engine.SetDrawAIPaths(!engine.GetDrawAIPaths());
		}
		break;

	case 'I':
		{
			if (GM_Roguelike* gameMode = engine.GetGameMode<GM_Roguelike>())
			{
				const Vector2Int& nextFloorRoomDoorPos = gameMode->GetNextFloorRoomDoorPosition();
				SetPosition(nextFloorRoomDoorPos);
			}
		}
		break;
	}
}

void PlayerPawn::UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition)
{
	assert(cameraComponent && "cameraComponent create fail..");
	cameraComponent->SetViewPosition(viewPosition);
}

//void PlayerPawn::ProcessFireInput()
//{
//	/* Projectile이 생성될 Offset 지정 */
//	if (fireInputValue != Vector2Int::Zero)
//	{
//		/* Projectile이 생성될 Offset 지정 */
//		const Vector2Int& spawnOffset = fireInputValue;
//		SetProjectileSpawnOffset(spawnOffset);
//
//		/* 조준 방향 지정(to offset) */
//		SetAimingDirection(static_cast<Vector2Float>(spawnOffset));
//
//		/* 조준 위치 지정(offset 위치로 향하는 방향 * Range) */
//		const Vector2Float aimingPosition = static_cast<Vector2Float>(GetWorldPosition()) + (GetAimingDirection() * GetFireRange());
//		SetAimingPostion(Vector2Int(static_cast<int>(round(aimingPosition.x)), static_cast<int>(round(aimingPosition.y))));
//	}
//
//	if (prevFireInputValue != fireInputValue)
//	{
//		if (fireInputValue == Vector2Int::Zero)
//		{
//			/* 이전에 발사중이었다가 중지 됨 */
//			SetProjectileAbilityTrigger(false);
//		}
//		else if (prevFireInputValue == Vector2Int::Zero)
//		{
//			/* 발사 중지 상태에서 발사 상태로 전환 */
//			SetProjectileAbilityTrigger(true);
//		}
//
//		prevFireInputValue = fireInputValue;
//	}
//}

void PlayerPawn::ProcessAimingInput()
{
	assert(inputComponent && "Invalid inputComponent");

	/* 월드 상의 마우스 좌표 */
	const Vector2Int& toMouseCursorPosInWorld = inputComponent->GetLastMosueCursorPos();
	
	/* 현재 위치 */
	const Vector2Int& currentPos = GetWorldPosition();

	Vector2Float newAimingDirection = Vector2Float::Zero;
	if (toMouseCursorPosInWorld == currentPos)
	{
		//마우스 위치가 플레이어 위에 위치하면 Up 반환
		newAimingDirection = Vector2Float::Up;
	}
	else
	{
		//플레이어 위치에서 마우스 커서가 가리키는 위치로 향하는 방향을 조준방향으로 삼음
		newAimingDirection = static_cast<Vector2Float>(toMouseCursorPosInWorld - currentPos);
		newAimingDirection.Normalize();
	}

	SetAimingDirection(newAimingDirection);
}

void PlayerPawn::ProcessMoveInput()
{
	assert(movementComponent && "Invalid movementComponent");

	movementComponent->SetLastMoveInputDireciton(moveInputDirection);
	moveInputDirection = eDirection::None;
}

//void PlayerPawn::SetProjectileAbilityTrigger(bool bTrigger)
//{
//	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
//	assert(abilitySystemComponent && "Invalid abilitySystemComponent");
//
//	for (AbilityObject::ABILITY_ID_TYPE grantProjectileAbilityID : grantProjectileAbilities)
//	{
//		AbilityObject* grantAbility = abilitySystemComponent->GetAbility<AbilityObject>(grantProjectileAbilityID);
//		if (!grantAbility)
//		{
//			continue;
//		}
//
//		if(bTrigger)
//		{
//			grantAbility->TriggerOn();
//		}
//		else
//		{
//			grantAbility->TriggerOff();
//		}
//	}
//}

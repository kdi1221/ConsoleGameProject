#include "PlayerPawn.h"
#include "Math/Color.h"
#include "Component/CameraComponent.h"
#include "Component/AbilitySystemComponent.h"
#include "Component/Attribute/PlayerAttributeComponent.h"
#include "Game/State/PlayerState/PlayerAbilityInfo.h"





#include "Actor/FieldItem/FieldSkillItem.h"
#include "Item/ItemData/ItemDataTable.h"
#include "Game/State/GameMode/GM_Roguelike.h"
#include "StaticLibrary/StaticFunctionLibrary.h"
#include <Engine/Engine.h>
#include <Engine/Config/ConfigBase.h>
#include <Core/Input.h>
#include <Math/Vector2Float.h>
#include <cassert>
#include <Windows.h>

using namespace Craft;

using InputTriggerType = InputComponent::FInputTrigger;

PlayerPawn::PlayerPawn(const Craft::Vector2Int& position)
	:super(position, L"☺", Color::Yellow, eTeamID::Player)
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
	inputComponent->AddInputCallback(VK_OEM_7, cheatInputTrigger);
	
	/* 이동 컴포넌트 */
	movementComponent = AddComponent<MovementComponent>(30.f);
	assert(movementComponent && "movementComponent create fail..");

	/* 카메라 컴포넌트 */
	cameraComponent = AddComponent<CameraComponent>();
	assert(cameraComponent && "cameraComponent create fail..");
}

void PlayerPawn::Initialize()
{
	super::Initialize();

	playerAttributeComponent = Cast<PlayerAttributeComponent>(attributeComponent);
	assert(playerAttributeComponent && "Invalid playerAttributeComponent");
	
	/* Mana Value 변화에 따른 콜백 설정 */
	playerAttributeComponent->SetChangeManaEventCallback(std::bind(&PlayerPawn::OnChangeManaValue, this, std::placeholders::_1, std::placeholders::_2));

	/* 카메라가 플레이어를 바라보게 함 */
	UpdateViewCameraPosition(GetWorldPosition());
}

void PlayerPawn::PreTick(float deltaTime)
{
	super::PreTick(deltaTime);

	/* 이동 입력 처리 */
	ProcessMoveInput();
}

void PlayerPawn::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	ProcessManaRegeneration(deltaTime);
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

std::shared_ptr<AttributeComponent> PlayerPawn::CreateAttributeComponent()
{
	return std::make_shared<PlayerAttributeComponent>();
}

void PlayerPawn::InitializeManaValue(const float currentMana, const float maxMana)
{
	assert(playerAttributeComponent && "Invalid attributeComponent");

	/* 초기 속성 값 설정 */
	playerAttributeComponent->InitialMana(currentMana, maxMana);
}

void PlayerPawn::SetManaChangeEventCallback(OnChangeManaType callback)
{
	onChangeManaEvent = callback;
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

	/* 이전에 저장해둔 Ability Cooldown 재지정 */
	const float cooldownElapsedTime = abilityInfo.GetSavedCooldownTime();
	if (cooldownElapsedTime > 0.f)
	{
		AbilityObject* grantedAbility = abilitySystemComponent->GetAbility<AbilityObject>(grantAbilityID);
		assert(grantedAbility && "Invalid grantedAbility");

		grantedAbility->SetCooldownElapsedTime(cooldownElapsedTime);
	}
}

void PlayerPawn::ConsumeMana(float consumeValue)
{
	assert(playerAttributeComponent && "Invalid playerAttributeComponent");
	playerAttributeComponent->SetCurrentMana(playerAttributeComponent->GetCurrentMana() - consumeValue);
}

void PlayerPawn::RestoreMana(float amount)
{
	assert(playerAttributeComponent && "Invalid playerAttributeComponent");
	playerAttributeComponent->SetCurrentMana(playerAttributeComponent->GetCurrentMana() + amount);
}

float PlayerPawn::GetCurrentMana() const
{
	assert(playerAttributeComponent && "Invalid playerAttributeComponent");
	return playerAttributeComponent->GetCurrentMana();
}

void PlayerPawn::OnChangeManaValue(float currentValue, float maxValue)
{
	if (onChangeManaEvent)
	{
		onChangeManaEvent(currentValue, maxValue);
	}
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

		newMoveDirection = StaticFunctionLibrary::GetNearestDirection(toCursorPosDirection);
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

	/* Ability 활성화 전 마우스 커서위치를 향한 Aiming Direction Update*/
	UpdateAimingDirectionToCursorPos();

	/* Input에 연결된 Ability ID에 대한 활성화 요청 */
	const ABILITY_ID_TYPE inputAbilityID = iterFindGrantAbilityID->second;
	abilitySystemComponent->ActivateAbility(inputAbilityID);
}

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

	case VK_OEM_7:
		{
			engine.SetDrawAIState(!engine.GetDrawAIState());
		}
		break;
	}
}

void PlayerPawn::ProcessManaRegeneration(float deltaTime)
{
	RestoreMana(manaRegenerationSpeed * deltaTime);
}

void PlayerPawn::UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition)
{
	assert(cameraComponent && "cameraComponent create fail..");
	cameraComponent->SetViewPosition(viewPosition);
}

void PlayerPawn::ProcessMoveInput()
{
	assert(movementComponent && "Invalid movementComponent");

	movementComponent->SetLastMoveInputDireciton(moveInputDirection);
	moveInputDirection = eDirection::None;
}

void PlayerPawn::UpdateAimingDirectionToCursorPos()
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
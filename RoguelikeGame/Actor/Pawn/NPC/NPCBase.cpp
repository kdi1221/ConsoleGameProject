#include "NPCBase.h"
#include "Navigation/NavigationTileMap.h"
#include "Component/BehaviorTree/BehaviorMeleeComponent.h"
#include "Component/BehaviorTree/BehaviorRangeComponent.h"
#include <Util/Util.h>
#include <Component/NavMovementComponent.h>
#include <Render/Renderer.h>
#include <Engine/Engine.h>

using namespace Craft;

NPCBase::NPCBase(const Vector2Int& position, 
				const std::wstring& image, 
				Color color, 
				float initialHealth, 
				float moveSpeed, 
				eMonsterPattern pattern, 
				RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, image, color, eTeamID::NPC)
	,initialiHealthValue(initialHealth)
	,spawnedRoomIndex(roomIndex)
{
	/* 경로 이동 컴포넌트 */
	navMovementComponent = AddComponent<NavMovementComponent>(moveSpeed);
	navMovementComponent->SetMoveFinishCallback(std::bind(&NPCBase::OnMoveFinish, this));
	navMovementComponent->SetMoveAbortCallback(std::bind(&NPCBase::OnMoveAbort, this));

	/* asc에 이벤트 설정 */
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponentPtr = GetAbilitySystemComponent();
	assert(abilitySystemComponentPtr && "Invalid abilitySystemComponent");
	abilitySystemComponentPtr->SetAbilityActivateCallback(std::bind(&NPCBase::OnActivateAbility, this, std::placeholders::_1, std::placeholders::_2));

	/* 몬스터 패턴 지정 */
	switch (pattern)
	{
	case eMonsterPattern::Melee:
		behaviorTreeComponent = AddComponent<BehaviorMeleeComponent>();
		break;

	case eMonsterPattern::Range:
		behaviorTreeComponent = AddComponent<BehaviorRangeComponent>();
		break;
	}
}

void NPCBase::Initialize()
{
	super::Initialize();

	/* 몬스터 Health 초기화 */
	InitializeHealthValue(initialiHealthValue, initialiHealthValue);
}

void NPCBase::Draw()
{
	super::Draw();

	/* 경로 확인 디버깅용 */
	if (Engine::Get().GetDrawAIPaths())
	{
		DrawMovePaths();
	}
}

void NPCBase::Destroy()
{
	/* Destroy전에 Stop Move 호출해야 함*/
	if (navMovementComponent)
	{
		navMovementComponent->StopMove();
	}

	super::Destroy();
}

void NPCBase::SetChaseTarget(std::weak_ptr<Pawn> target)
{
	if (behaviorTreeComponent)
	{
		behaviorTreeComponent->SetChaseTarget(target);
	}
}

bool NPCBase::BeginPathfindingToTargetMove(const Vector2Int& targetPosition)
{
	//목표 주위 위치로 이동 시작 
	assert(navMovementComponent && "Invalid NavMovementComponent");
	return navMovementComponent->StartMove(targetPosition);
}

void NPCBase::StopMove()
{
	assert(navMovementComponent && "Invalid NavMovementComponent");
	navMovementComponent->StopMove();
}

void NPCBase::ExecuteAttack()
{

}

void NPCBase::OnNotifyAttackFrame(const AbilityObject& ability)
{

}

std::shared_ptr<Pawn> NPCBase::GetChaseTarget() const
{
	if (!behaviorTreeComponent)
	{
		return nullptr;
	}

	return behaviorTreeComponent->GetChaseTarget();
}

std::shared_ptr<BehaviorTreeComponent> NPCBase::GetBehaviorTreeComponent() const
{
	return behaviorTreeComponent;
}

void NPCBase::DrawMovePaths()
{
	if (!navMovementComponent)
	{
		return;
	}

	const eFindPathResult findPathResult = navMovementComponent->GetFindPathResult();
	if (findPathResult == eFindPathResult::Success || findPathResult == eFindPathResult::Throttled)
	{
		Renderer& renderer = Renderer::Get();
		const Color drawPathColor = findPathResult == eFindPathResult::Success ? Color::BG_LightGreen : Color::BG_LightBlue;

		navMovementComponent->Foreach_Path([&](const Vector2Int& path)
			{
				renderer.Submit(L" ", path, drawPathColor, static_cast<int>(eRenderSortingOrder::MapObject));
			}
		);
	}
}

void NPCBase::OnMoveFinish()
{
	//이동 목적지에 도착한 경우
	if (behaviorTreeComponent)
	{
		behaviorTreeComponent->OnMoveFinish();
	}
}

void NPCBase::OnMoveAbort()
{
	//다른 객체에 충돌하여 멈춘경우 : 그 객체가 타겟일수 있으므로 타겟과의 거리 및 상태 체크
	if (behaviorTreeComponent)
	{
		behaviorTreeComponent->OnMoveAbort();
	}
}

void NPCBase::OnActivateAbility(const AbilityObject& ability, bool bActivate)
{
	/* 실행된 Ability가 종료되면 비헤이비어트리에 알림 */
	if (!bActivate)
	{
		if (behaviorTreeComponent)
		{
			behaviorTreeComponent->OnEndAbility(ability);
		}
	}
}

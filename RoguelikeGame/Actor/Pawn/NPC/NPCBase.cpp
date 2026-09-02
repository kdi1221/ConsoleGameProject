#include "NPCBase.h"
#include "Engine/Engine.h"
#include "Navigation/NavigationTileMap.h"
#include <Util/Util.h>
#include <Component/NavMovementComponent.h>
#include "Render/Renderer.h"

using namespace Craft;

const Vector2Int NPCBase::INVALID_LAST_POS = Vector2Int(-1, -1);

NPCBase::NPCBase(const Craft::Vector2Int& position, 
				const std::wstring& image, 
				Craft::Color color, 
				float initialHealth,
				RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
				float moveDelay,
				float chaseDelay)
	:super(position, image, color, eTeamID::NPC)
	,initialiHealthValue(initialHealth)
	,spawnedRoomIndex(roomIndex)
{
	navMovementComponent = AddComponent<NavMovementComponent>(10.f);
	navMovementComponent->SetMoveFinishCallback(std::bind(&NPCBase::OnMoveFinish, this));
	navMovementComponent->SetMoveAbortCallback(std::bind(&NPCBase::OnMoveAbort, this));
}

void NPCBase::Initialize()
{
	super::Initialize();

	/* 몬스터 Health 초기화 */
	InitializeHealthValue(initialiHealthValue, initialiHealthValue);
}

void NPCBase::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//TODO : 테스트를 위해 AI 비활성화
	//return;

	switch (behaviorState)
	{
	case eMonsterBehavior::Idle:
		{
			OnBehaviorIdle(deltaTime);
		}
		break;

	case eMonsterBehavior::TargetChase:
		{
			OnBehaviorChaseTarget(deltaTime);
		}
		break;

	case eMonsterBehavior::Attack:
		{
			OnBehaviorAttack(deltaTime);
		}
		break;
	}
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
	assert(!target.expired() && "target invalid..");
	chaseTarget = target;
	SetBehaviorState(eMonsterBehavior::TargetChase);
}

void NPCBase::SetBehaviorState(eMonsterBehavior newState)
{
	eMonsterBehavior beforeState = behaviorState;
	behaviorState = newState;

	if (beforeState != behaviorState)
	{
		/* 이전 상태 정리 */
		switch (beforeState)
		{
		case eMonsterBehavior::Idle:
		{
			//OutputDebugStringA("On Idle Exit\n");
		}
		break;

		case eMonsterBehavior::TargetChase:
		{
			timerFindChasePathDelay.Reset();
			StopMove();
		}
		break;

		case eMonsterBehavior::Attack:
		{
			AttackAbilitiesTriggerOFF();
		}
		break;
		}

		/* 새로운 상태 시작 */
		switch (behaviorState)
		{
		case eMonsterBehavior::Idle:
		{
			//OutputDebugStringA("On Idle Begin\n");
		}
		break;

		case eMonsterBehavior::TargetChase:
		{
			//다음 타겟 추적 딜레이 설정 
			SetTargetChaseDelayTime();
		}
		break;

		case eMonsterBehavior::Attack:
		{
			AttackAbilitiesTriggerON();
		}
		break;
		}
	}
}

bool NPCBase::BeginPathfindingToTarget()
{
	std::shared_ptr<Pawn> target = chaseTarget.lock();
	assert(target && "Invalid target");

	const Vector2Int& startPos = GetWorldPosition();
	const Vector2Int& targetPos = target->GetWorldPosition();

	//Target 주위 위치 중 갈수 있는 위치 구하기
	std::vector<Vector2Int> availablePosition;
	GetAvailableChaseTargetPosition(targetPos, availablePosition);
	if (availablePosition.empty())
	{
		//타겟 주변에 이동할 곳이 없는경우 진행하지 않음(이후 이동할곳이 생길때까지 대기)
		return false;
	}

	for (size_t i = availablePosition.size() - 1; i > 0; --i)
	{
		//0 ~ i 까지 인덱스 중 랜덤하게 선택
		const int j = Util::RandomRange(0, static_cast<int>(i));

		//현재 인덱스와 랜덤하게 결정된 인덱스를 교환해서 뒤섞는다.
		std::swap(availablePosition[i], availablePosition[j]);
	}

	//목표 주위 위치로 이동 시작 
	assert(navMovementComponent && "Invalid NavMovementComponent");
	return navMovementComponent->StartMove(availablePosition[0]);
}

void NPCBase::StopMove()
{
	assert(navMovementComponent && "Invalid NavMovementComponent");
	navMovementComponent->StopMove();
}

/* 타겟을 향해 추적중 타겟의 상태 및 거리에 따라 다음 상태로 전환한다. */
bool NPCBase::TransitionNextStateWhileChase()
{
	std::shared_ptr<Pawn> targetPawn = chaseTarget.lock();
	if (!targetPawn || targetPawn->IsDeath())
	{
		//Idle 상태로 전환한다.
		SetBehaviorState(eMonsterBehavior::Idle);
		return true;
	}
	else
	{
		//사정거리 안에있는지 확인
		if (IsTargetAttackRange(targetPawn))
		{
			//공격 실행
			SetBehaviorState(eMonsterBehavior::Attack);
			return true;
		}
	}

	return false;
}

void NPCBase::SetTargetChaseDelayTime()
{
	/* 타겟 추적을 위한 경로 탐색 딜레이 무작위 지정 */
	const float nextFindPathDelay = Util::RandomRange(minFindPathdelay, maxFindPathdelay);
	timerFindChasePathDelay.SetTargetTime(nextFindPathDelay);
	timerFindChasePathDelay.Reset();
}

void NPCBase::SetTargetChaseNextTick()
{
	/* 다음 프레임에서 경로 탐색이 실행되도록 타이머 타임 끝으로 설정 */
	timerFindChasePathDelay.ReserveNextTick();
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

void NPCBase::OnBehaviorIdle(float deltaTime)
{
	// TODO : 현재로써는 타겟 사망이후에나 들어오므로 따로 처리할 부분 없음
}

void NPCBase::OnBehaviorChaseTarget(float deltaTime)
{
	//타겟의 상태 및 거리를 확인하여 다른 상태로 전이되었으면 타겟 추적 실행 안함
	if (TransitionNextStateWhileChase())
	{
		return;
	}

	/* 타겟 추적 타이머 딜레이 갱신(일정 딜레이마다 타겟과 주변의 환경변화를 인지해서 경로를 새로 잡는다) */
	timerFindChasePathDelay.Tick(deltaTime);
	if (timerFindChasePathDelay.IsTimeOut())
	{
		StopMove();
		/* 일정 딜레이마다 타겟 추적 경로를 갱신한다. */
		BeginPathfindingToTarget();

		/* 다음 이동 체크 딜레이 설정 */
		SetTargetChaseDelayTime();
	}
}

void NPCBase::OnBehaviorAttack(float deltaTime)
{
	//Target이 살아있는지 확인 
	std::shared_ptr<Pawn> targetPawn = chaseTarget.lock();
	if (!targetPawn || targetPawn->IsDeath())
	{
		//Idle 상태로 전환한다.
		SetBehaviorState(eMonsterBehavior::Idle);
	}
	else if (!IsTargetAttackRange(targetPawn)) //사정거리 안에있는지 확인
	{
		//사정거리 밖이면 다시 타겟 추적
		SetBehaviorState(eMonsterBehavior::TargetChase);
	}
}

void NPCBase::OnMoveFinish()
{
	//이동 목적지에 도착한 경우

	//현재 타겟 추적중인 상태라면 
	if (eMonsterBehavior::TargetChase == GetBehaviorState())
	{
		// 타겟의 상태 및 거리 체크 후 상태 변화가 없다면 
		if (!TransitionNextStateWhileChase())
		{
			//다음 프레임에서 다시 추적
			SetTargetChaseNextTick();
		}
	}
}

void NPCBase::OnMoveAbort()
{
	//다른 객체에 충돌하여 멈춘경우 : 그 객체가 타겟일수 있으므로 타겟과의 거리 및 상태 체크
	
	//현재 타겟 추적중인 상태라면 
	if (eMonsterBehavior::TargetChase == GetBehaviorState())
	{
		// 타겟의 상태 및 거리 체크 후 상태 변화가 없다면 
		if (!TransitionNextStateWhileChase())
		{
			//다음 프레임에서 다시 추적
			SetTargetChaseNextTick();
		}
	}
}

#include "NPCBase.h"
#include "Engine/Engine.h"
#include "Navigation/NavigationTileMap.h"
#include <Util/Util.h>
#include <Component/PathMoveComponent.h>
#include "Render/Renderer.h"

using namespace Craft;

NPCBase::NPCBase(const Craft::Vector2Int& position, 
				const std::wstring& image, 
				Craft::Color color, 
				int CollisionWidth,
				int initialHealth,
				RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
				float moveDelay,
				float ChaseDelay)
	:super(position, image, color, CollisionWidth, initialHealth, eTeamID::NPC)
	,spawnedRoomIndex(roomIndex)
{
	pathMoveComponent = AddComponent<PathMoveComponent>(moveDelay, true);
	pathMoveComponent->SetMoveFinishCallback(std::bind(&NPCBase::OnMoveFinish, this));
	pathMoveComponent->SetMoveAbortCallback(std::bind(&NPCBase::OnMoveAbort, this));

	timerFindChasePathDelay.SetTargetTime(ChaseDelay);
}

void NPCBase::Tick(float deltaTime)
{
	super::Tick(deltaTime);

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

	/* 경로 확인 테스트용 */
	for (const Vector2Int& pathCoord : debugMovePaths)
	{
		Renderer::Get().Submit(L" ", pathCoord, Color::BG_LightGreen, static_cast<int>(eRenderSortingOrder::MapObject));
	}
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
				
			}
			break;
		}

		/* 새로운 상태 시작 */
		switch (behaviorState)
		{
		case eMonsterBehavior::Idle:
			{
				
			}
			break;

		case eMonsterBehavior::TargetChase:
			{
				timerFindChasePathDelay.Reset();
			}
			break;

		case eMonsterBehavior::Attack:
			{
				
			}
			break;
		}
	}
}

void NPCBase::BeginPathfindingToTarget()
{
	std::shared_ptr<Pawn> target = chaseTarget.lock();
	assert(target && "Invalid target");

	const NavigationTilemap& navigationSystem = Engine::Get().GetNavigationSystem<NavigationTilemap>();

	const Vector2Int& startPos = GetWorldPosition();
	const Vector2Int& targetPos = target->GetWorldPosition();

	//Target 주위 위치 중 갈수 있는 위치 구하기
	std::vector<Vector2Int> availablePosition;
	for (int y = -1; y != 1; ++y)
	{
		for (int x = -1; x != 1; ++x)
		{
			if (y == 0 && x == 0)
			{
				continue;
			}

			const Vector2Int checkPos(targetPos + Vector2Int(x, y));
			if (!navigationSystem.CanNextMove(shared_from_this(), checkPos))
			{
				continue;
			}

			availablePosition.emplace_back(checkPos);
		}
	}

	if (availablePosition.empty())
	{
		return;
	}

	for (size_t i = availablePosition.size() - 1; i > 0; --i)
	{
		//0 ~ i 까지 인덱스 중 랜덤하게 선택
		const int j = Util::RandomRange(0, static_cast<int>(i));

		//현재 인덱스와 랜덤하게 결정된 인덱스를 교환해서 뒤섞는다.
		std::swap(availablePosition[i], availablePosition[j]);
	}

	/* 타겟 주변위치를 향한 경로를 탐색한다. */
	std::vector<Craft::Vector2Int> movePaths;
	if (!navigationSystem.FindPath(shared_from_this(), startPos, availablePosition[0], movePaths))
	{
		return;
	}

	//디버깅용
	debugMovePaths = movePaths;

	//movePaths기반으로 이동 시작
	assert(pathMoveComponent && "Invalid pathMoveComponent");
	pathMoveComponent->StartMove(std::move(movePaths));
}

void NPCBase::StopMove()
{
	assert(pathMoveComponent && "Invalid pathMoveComponent");
	pathMoveComponent->StopPathMove();
}

void NPCBase::OnBehaviorIdle(float deltaTime)
{
	// TODO : 현재로써는 타겟 사망외에 따로 처리할 부분 없음
}

void NPCBase::OnBehaviorChaseTarget(float deltaTime)
{
	/* 타겟이 더이상 유효하지 않거나 사망했으면 */
	std::shared_ptr<Pawn> targetPawn = chaseTarget.lock();
	if (!targetPawn || targetPawn->IsDeath())
	{
		//Idle 상태로 전환한다.
		SetBehaviorState(eMonsterBehavior::Idle);
		return;
	}

	/* TODO : 타겟이 공격범위 안에 있는경우 이동 중단 후 공격으로 전환 */

	/* 타겟 추적 타이머 딜레이 갱신 */
	timerFindChasePathDelay.Tick(deltaTime);
	if (timerFindChasePathDelay.IsTimeOut())
	{
		BeginPathfindingToTarget();
		timerFindChasePathDelay.Reset();
	}
}

void NPCBase::OnBehaviorAttack(float deltaTime)
{
	//TODO : 공격 도중 Target이 사정거리 밖으로 벗어나면 Chase를 시작한다.
}

void NPCBase::OnMoveFinish()
{
	OutputDebugStringA("Move Finish\n");

	//TODO : 이동이 끝난뒤 Target이 사정거리 안에있는지 확인해서 공격 또는 다시 Chase를 시작한다.
}

void NPCBase::OnMoveAbort()
{
	//OutputDebugStringA("Move Abort\n");

	// TODO : 다른 객체에 충돌된 경우 Target이 사정거리 안에 있는지 확인해서 공격 또는 다시 Chase를 시작한다.
	BeginPathfindingToTarget();
	timerFindChasePathDelay.Reset();
}

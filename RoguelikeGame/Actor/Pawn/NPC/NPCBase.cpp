#include "NPCBase.h"
#include "Engine/Engine.h"
#include "Navigation/NavigationTileMap.h"
#include <Util/Util.h>
#include <Component/PathMoveComponent.h>
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
	pathMoveComponent = AddComponent<PathMoveComponent>(moveDelay, true);
	pathMoveComponent->SetMoveFinishCallback(std::bind(&NPCBase::OnMoveFinish, this));
	pathMoveComponent->SetMoveAbortCallback(std::bind(&NPCBase::OnMoveAbort, this));

	timerFindChasePathDelay.SetTargetTime(chaseDelay);
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
	return;

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
		if (pathMoveComponent)
		{
			pathMoveComponent->Foreach_Path([&](const Vector2Int& path)
				{
					Renderer::Get().Submit(L" ", path, Color::BG_LightGreen, static_cast<int>(eRenderSortingOrder::MapObject));
				}
			);
		}
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
				AttackAbilitiesTriggerOFF();
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
				//다음 프레임에 바로 대상을 쫓을 수 있도록 함
				timerFindChasePathDelay.ReserveNextTick();
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

void NPCBase::BeginPathfindingToTarget()
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
		//타겟 주변에 이동할 곳이 없는경우 정지
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
	const NavigationTilemap& navigationSystem = Engine::Get().GetNavigationSystem<NavigationTilemap>();
	if (!navigationSystem.FindPath(shared_from_this(), startPos, availablePosition[0], movePaths))
	{
		// 이동할 경로를 찾지 못하면 정지
		return;
	}

	//마지막에 추적한 타겟의 위치를 기록해둔다.
	lastChaseTargetPos = targetPos;

	//movePaths기반으로 이동 시작
	assert(pathMoveComponent && "Invalid pathMoveComponent");
	pathMoveComponent->StartMove(std::move(movePaths));
}

void NPCBase::StopMove()
{
	assert(pathMoveComponent && "Invalid pathMoveComponent");
	pathMoveComponent->StopPathMove();
}

void NPCBase::CheckTargetWhileChase(bool bForcePathUpdate)
{
	assert(pathMoveComponent && "Invalid pathMoveComponent");

	//이동이 끝난뒤 Target이 살아있는지 확인 
	std::shared_ptr<Pawn> targetPawn = chaseTarget.lock();
	if (!targetPawn || targetPawn->IsDeath())
	{
		//Idle 상태로 전환한다.
		SetBehaviorState(eMonsterBehavior::Idle);
	}
	else
	{
		//사정거리 안에있는지 확인
		if (IsTargetAttackRange(targetPawn))
		{
			//공격 실행
			SetBehaviorState(eMonsterBehavior::Attack);
		}
		else
		{
			/* 강제 경로업데이트 또는 기존에 이동하려는 목적지가 타겟의 위치가 아닌경우에만 경로 새로 업데이트*/
			const bool isPathUpdate = bForcePathUpdate || lastChaseTargetPos != targetPawn->GetWorldPosition();

			if (isPathUpdate)
			{
				//다시 이동 시작
				BeginPathfindingToTarget();
			}

			timerFindChasePathDelay.Reset();
		}
	}
}

void NPCBase::OnBehaviorIdle(float deltaTime)
{
	// TODO : 현재로써는 타겟 사망이후에나 들어오므로 따로 처리할 부분 없음
}

void NPCBase::OnBehaviorChaseTarget(float deltaTime)
{
	/* 타겟 추적 타이머 딜레이 갱신(일정 딜레이마다 타겟과 주변의 환경변화를 인지해서 경로를 새로 잡는다) */
	timerFindChasePathDelay.Tick(deltaTime);
	if (timerFindChasePathDelay.IsTimeOut())
	{
		/* 일정 딜레이마다 타겟과 주변 환경 변화를 인지해서 경로를 새로잡거나 공격범위 안에 있으면 공격한다. */
		CheckTargetWhileChase(false);
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
	//목적지 도착 : 타겟이 거리에 있으므로 타겟과의 거리 및 상태 체크
	CheckTargetWhileChase(true);
}

void NPCBase::OnMoveAbort()
{
	//다른 객체에 충돌하여 멈춘경우 : 그 객체가 타겟일수 있으므로 타겟과의 거리 및 상태 체크
	CheckTargetWhileChase(true);
}

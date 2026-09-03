#include "BehaviorTreeComponent.h"
#include "Actor/Pawn/NPC/NPCBase.h"
#include <Render/Renderer.h>
#include <Util/Util.h>
#include <Engine/Engine.h>
#include <Windows.h>
#include <cassert>

using namespace Craft;

BehaviorTreeComponent::BehaviorTreeComponent()
{

}

void BehaviorTreeComponent::BeginPlay()
{
	super::BeginPlay();

	/* Owner 지정 */
	std::shared_ptr<NPCBase> ownerNPCPtr = Cast<NPCBase>(GetOwner());
	assert(ownerNPCPtr && "Invalid ownerNPCPtr");
	ownerNPC = ownerNPCPtr;
}

void BehaviorTreeComponent::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	OnBehaviorUpdate(deltaTime);
}

void BehaviorTreeComponent::Draw()
{
	super::Draw();

	if (Engine::Get().GetDrawAIState())
	{
		DrawCurrentState();
	}
}

void BehaviorTreeComponent::SetChaseTarget(std::weak_ptr<Pawn> target)
{
	assert(!target.expired() && "target invalid..");
	chaseTarget = target;
	SetBehaviorState(eBehaviorState::TargetChase);
}

void BehaviorTreeComponent::OnMoveFinish()
{
	if (eBehaviorState::TargetChase == GetBehaviorState())
	{
		// 타겟의 상태 및 거리 체크 후 상태 변화가 없다면 
		if (!TransitionNextStateWhileChase())
		{
			//다음 프레임에서 다시 추적
			SetTargetChaseNextTick();
		}
	}
}

void BehaviorTreeComponent::OnMoveAbort()
{
	if (eBehaviorState::TargetChase == GetBehaviorState())
	{
		// 타겟의 상태 및 거리 체크 후 상태 변화가 없다면 
		if (!TransitionNextStateWhileChase())
		{
			//다음 프레임에서 다시 추적
			SetTargetChaseNextTick();
		}
	}
}

void BehaviorTreeComponent::OnEndAbility(const AbilityObject& ability)
{
	//이전 공격 Ability 종료

	// Target이 살아있는지 확인 
	std::shared_ptr<Pawn> targetPawn = chaseTarget.lock();
	if (!targetPawn || targetPawn->IsDeath())
	{
		//Idle 상태로 전환한다.
		SetBehaviorState(eBehaviorState::Idle);
	}
	else if (!IsTargetAttackRange(targetPawn)) //사정거리 안에있는지 확인
	{
		//사정거리 밖이면 다시 타겟 추적
		SetBehaviorState(eBehaviorState::TargetChase);
	}
	else
	{
		//타겟이 살아있고 사정거리 안에 들어와있으므로 다음번에 다시 Ability를 활성화한다.
		bActivateAttackAbility = false;
	}
}

std::shared_ptr<Pawn> BehaviorTreeComponent::GetChaseTarget() const
{
	return chaseTarget.lock();
}

std::shared_ptr<NPCBase> BehaviorTreeComponent::GetNPCOwner() const
{
	return ownerNPC.lock();
}

void BehaviorTreeComponent::SetBehaviorState(eBehaviorState newState)
{
	eBehaviorState beforeState = behaviorState;
	behaviorState = newState;

	if (beforeState != behaviorState)
	{
		/* 이전 상태 정리 */
		ClearPreviouseBehaviorState(beforeState);

		/* 새로운 상태 시작 */
		BeginNewBehaviorState();
	}
}

void BehaviorTreeComponent::OnBehaviorUpdate(float deltaTime)
{
	switch (behaviorState)
	{
	case eBehaviorState::Idle:
		{
			OnBehaviorIdle(deltaTime);
		}
		break;

	case eBehaviorState::TargetChase:
		{
			OnBehaviorChaseTarget(deltaTime);
		}
		break;

	case eBehaviorState::Attack:
		{
			OnBehaviorAttack(deltaTime);
		}
		break;
	}
}

void BehaviorTreeComponent::ExecuteAttack()
{
	std::shared_ptr<NPCBase> ownerNPCPtr = ownerNPC.lock();
	if (!ownerNPCPtr)
	{
		return;
	}

	ownerNPCPtr->ExecuteAttack();

	bActivateAttackAbility = true;
}

bool BehaviorTreeComponent::BeginPathfindingToTarget()
{
	std::shared_ptr<Pawn> chaseTargetPtr = chaseTarget.lock();
	std::shared_ptr<NPCBase> ownerNPCPtr = ownerNPC.lock();

	if (chaseTargetPtr && ownerNPCPtr)
	{
		const Vector2Int& targetPosition = chaseTargetPtr->GetWorldPosition();

		//Target 주위 위치 중 갈수 있는 위치 후보 리스트 구하기
		std::vector<Vector2Int> availablePosition;
		GetAvailableChaseTargetPosition(targetPosition, availablePosition);
		if (availablePosition.empty())
		{
			return false;
		}

		for (size_t i = availablePosition.size() - 1; i > 0; --i)
		{
			//0 ~ i 까지 인덱스 중 랜덤하게 선택
			const int j = Util::RandomRange(0, static_cast<int>(i));

			//현재 인덱스와 랜덤하게 결정된 인덱스를 교환해서 뒤섞는다.
			std::swap(availablePosition[i], availablePosition[j]);
		}

		return ownerNPCPtr->BeginPathfindingToTargetMove(availablePosition[0]);
	}
	else
	{
		return false;
	}
}

void BehaviorTreeComponent::StopMove()
{
	if (std::shared_ptr<NPCBase> ownerNPCPtr = ownerNPC.lock())
	{
		ownerNPCPtr->StopMove();
	}
}

bool BehaviorTreeComponent::TransitionNextStateWhileChase()
{
	std::shared_ptr<Pawn> targetPawn = chaseTarget.lock();
	if (!targetPawn || targetPawn->IsDeath())
	{
		//Idle 상태로 전환한다.
		SetBehaviorState(eBehaviorState::Idle);
		return true;
	}
	else
	{
		//사정거리 안에있는지 확인
		if (IsTargetAttackRange(targetPawn))
		{
			//공격 실행
			SetBehaviorState(eBehaviorState::Attack);
			return true;
		}
	}

	return false;
}

void BehaviorTreeComponent::SetTargetChaseDelayTime()
{
	/* 타겟 추적을 위한 경로 탐색 딜레이 무작위 지정 */
	const float nextFindPathDelay = Util::RandomRange(minFindPathdelay, maxFindPathdelay);
	timerFindChasePathDelay.SetTargetTime(nextFindPathDelay);
	timerFindChasePathDelay.Reset();
}

void BehaviorTreeComponent::SetTargetChaseNextTick()
{
	/* 다음 프레임에서 경로 탐색이 실행되도록 타이머 타임 끝으로 설정 */
	timerFindChasePathDelay.ReserveNextTick();
}

void BehaviorTreeComponent::DrawCurrentState()
{
	std::shared_ptr<NPCBase> ownerNPCPtr = ownerNPC.lock();
	if (!ownerNPCPtr)
	{
		return;
	}

	Renderer& renderer = Renderer::Get();

	std::wstring stringState;

	switch (GetBehaviorState())
	{
	case eBehaviorState::Idle:
		stringState = L"Idle";
		break;

	case eBehaviorState::TargetChase:
		stringState = L"TargetChase";
		break;

	case eBehaviorState::Attack:
		stringState = L"Attack";
		break;
	}

	renderer.Submit(stringState, ownerNPCPtr->GetWorldPosition() + Vector2Int::One, Color::Green, static_cast<int>(eRenderSortingOrder::Pawn));
}

void BehaviorTreeComponent::ClearPreviouseBehaviorState(eBehaviorState prevState)
{
	/* 이전 상태 정리 */
	switch (prevState)
	{
	case eBehaviorState::Idle:
		{
			//OutputDebugStringA("On Idle Exit\n");
		}
		break;

	case eBehaviorState::TargetChase:
		{
			timerFindChasePathDelay.Reset();
			StopMove();
		}
		break;

	case eBehaviorState::Attack:
		{
			bActivateAttackAbility = false;
			//OutputDebugStringA("TODO : Attack End\n");
			//AttackAbilitiesTriggerOFF();
		}
		break;
	}
}

void BehaviorTreeComponent::BeginNewBehaviorState()
{
	switch (behaviorState)
	{
	case eBehaviorState::Idle:
		{
			//OutputDebugStringA("On Idle Begin\n");
		}
		break;

	case eBehaviorState::TargetChase:
		{
			//다음 타겟 추적 딜레이 설정 
			SetTargetChaseDelayTime();
		}
		break;

	case eBehaviorState::Attack:
		{
			ExecuteAttack();
		}
		break;
	}
}

void BehaviorTreeComponent::OnBehaviorIdle(float deltaTime)
{
	// TODO : 현재로써는 최초 생성 및 타겟 사망이후에나 들어오므로 따로 처리할 부분 없음
}

void BehaviorTreeComponent::OnBehaviorChaseTarget(float deltaTime)
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
		/* 기존 이동 정지 */
		StopMove();

		/* 일정 딜레이마다 타겟 추적 경로를 갱신한다. */
		BeginPathfindingToTarget();

		/* 다음 이동 체크 딜레이 설정 */
		SetTargetChaseDelayTime();
	}
}

void BehaviorTreeComponent::OnBehaviorAttack(float deltaTime)
{
	if (!bActivateAttackAbility)
	{
		ExecuteAttack();
	}
}
#pragma once

#include <Core/Core.h>
#include <Core/CraftObject.h>
#include <Math/Vector2Int.h>
#include <vector>
#include <Defines/Defines.h>
#include <Defines/Enums.h>

namespace Craft
{
	class Actor;
	class Level;
	class NavMovementComponent;

	/* 게임 내 길찾기 시스템 기반 */
	class CRAFT_API NavigationBase : public CraftObject
	{
		TYPE_DECLARATIONS(NavigationBase, CraftObject)

	public:
		/* 경로 탐색 횟수 최대 제한*/
		static const int LIMIT_PATH_FIND_NUM;

		/* 타겟 위치까지 선을 그었을때 충돌되는 대상이 있는지 여부 반환 */
		enum class eCheckMoveTargetResult
		{
			/* 기본값 */
			None,

			/* 타겟 위치로 정상 이동 가능 */
			Success,

			/* 타겟 위치로 이동 중 벽에 부딪침 */
			BlockWall,

			/* 타겟 위치로 이동 중 다른 충돌 Actor와 부딪침 */
			BlockActor,

			/* 대각 이동 Block*/
			BlockDiagonal,

			/* 기타 알 수 없는 이유로 실패 */
			Unknown,
		};

	public:
		NavigationBase();
		virtual ~NavigationBase() = default;

	public:
		/* 모아둔 경로 찾기 요청 처리 */
		virtual void ProcessPathFindRequests();

		/* 경로찾기 요청 */
		virtual eFindPathResult RequestFindPath(std::shared_ptr<NavMovementComponent> requester,
												const Vector2Int& startPos,
												const Vector2Int& endPos,
												RequestPathHandleType& outRequestPathHandle);

		/* 경로 찾기 요청 취소 */
		virtual void CancelFindPathRequest(std::shared_ptr<NavMovementComponent> requester);


		/* 시작위치 - 종료위치를 연결하는 경로 생성 반환 */
		virtual eFindPathResult FindPath(std::shared_ptr<Actor> agent,
								const Vector2Int& startPos, 
								const Vector2Int& endPos, 
								std::vector<Vector2Int>& resultPath) const;

		/* agent가 prevPosition에서 nextPosition으로 이동가능한지 체크 */
		virtual bool SimulatePreviousToNextMove(std::shared_ptr<Actor> agent,
												const Vector2Int& prevPosition, 
												const Vector2Int& nextPosition) const;

		/* 해당 지점으로 이동 가능한지 여부 반환 */
		virtual bool CanNextMove(std::shared_ptr<Actor> agent, const Vector2Int& checkPos) const;

		/* 타겟까지 이동 시뮬레이션 도중 충돌되는 대상이 있는지 여부 반환 */
		virtual eCheckMoveTargetResult CheckEnableMoveToTargetPosition(std::shared_ptr<Craft::Actor> agent,
																	const Craft::Vector2Int& checkPos,
																	Vector2Int& enableMovePosition) const;

	public:
		/* 기존 레벨이 정리될 때 호출 */
		virtual void ResetCurrentLevel();

		/* 새로운 레벨이 설정될 때 호출 */
		virtual void SetCurrentLevel(std::weak_ptr<Level> level);

	protected:
		// 현재 활성화된 레벨 캐스팅 반환
		template<typename T, typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		std::shared_ptr<T> GetCurrentLevel() const
		{
			return Cast<T>(currentLevel.lock());
		}

	protected:
		/* 경로 찾기 요청에 대한 핸들값 생성 */
		RequestPathHandleType GenerateRequestPathHandle() const;

	private:
		/* 현재 활성화된 레벨 */
		std::weak_ptr<Level> currentLevel;
	};
}

using CheckMoveResultType = Craft::NavigationBase::eCheckMoveTargetResult;



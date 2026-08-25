#pragma once

#include <Core/Core.h>
#include <Core/CraftObject.h>
#include <Math/Vector2Int.h>
#include <vector>

namespace Craft
{
	class Actor;
	class Level;

	/* 게임 내 길찾기 시스템 기반 */
	class CRAFT_API NavigationBase : public CraftObject
	{
		TYPE_DECLARATIONS(NavigationBase, CraftObject)

	public:

		/* 경로 */
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

			/* 기타 알 수 없는 이유로 실패 */
			Unknown,
		};

	public:
		NavigationBase();
		virtual ~NavigationBase() = default;

	public:
		/* 시작위치 - 종료위치를 연결하는 경로 생성 반환 */
		virtual bool FindPath(std::shared_ptr<Actor> agent,
								const Vector2Int& startPos, 
								const Vector2Int& endPos, 
								std::vector<Vector2Int>& resultPath) const;

		/* 해당 지점으로 이동 가능한지 여부 반환 */
		virtual bool CanNextMove(std::shared_ptr<Actor> agent, const Vector2Int& checkPos) const;

		/* 타겟까지 이동 시뮬레이션 도중 충돌되는 대상이 있는지 여부 반환 */
		virtual eCheckMoveTargetResult CheckEnableMoveToTargetPosition(std::shared_ptr<Craft::Actor> agent,
																	const Craft::Vector2Int& checkPos,
																	Vector2Int& enableMovePosition) const;

	public:
		void SetCurrentLevel(std::weak_ptr<Level> level);

	protected:
		// 현재 활성화된 레벨 캐스팅 반환
		template<typename T, typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		std::shared_ptr<T> GetCurrentLevel() const
		{
			return Cast<T>(currentLevel.lock());
		}

	private:
		// 현재 활성화된 레벨
		std::weak_ptr<Level> currentLevel;
	};
}

using CheckMoveResultType = Craft::NavigationBase::eCheckMoveTargetResult;



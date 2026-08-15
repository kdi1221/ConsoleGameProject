#pragma once

#include "Core/CraftObject.h"
#include "Math/Vector2Int.h"

namespace Craft
{
	/* UI 표시할 Widget Base */
	class CRAFT_API Widget : public CraftObject
	{
		TYPE_DECLARATIONS(Widget, CraftObject)

	public:
		Widget(const Vector2Int& positionLT, int widgetWidth, int widgetHeight);
		virtual ~Widget();

	public:
		//프레임 이벤트 함수.
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();
		virtual void Destroy();

	public:
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }
		inline const Vector2Int& GetPosition() const { return position; }
		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }
		inline int GetRenderSortingOrder() const { return renderSortingOrder; }

	private:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 활성화 여부 플래그.
		bool isActive = true;

		// 삭제 예약 설정 플래그.
		bool hasExpired = false;

		//좌상단 기준 위치
		Vector2Int position = Vector2Int::Zero;

		//길이
		int width = 0;

		//높이
		int height = 0;

		//깊이버퍼 순서
		int renderSortingOrder = 0;
	};
}



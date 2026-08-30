#pragma once

#include "Core/CraftObject.h"
#include "Math/Vector2Int.h"
#include <vector>

namespace Craft
{
	class Level;

	/* UI 표시할 Widget Base */
	class CRAFT_API Widget : public CraftObject,
							 public std::enable_shared_from_this<Widget>
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
		void SetActive(bool bActive);
		void SetRenderSortingOrder(int order);
		void SetOwnerLevel(std::weak_ptr<Level> newOwner);
		void SetOwnerWidget(std::weak_ptr<Widget> newOwner);

	public:
		inline std::shared_ptr<Level> GetOwnerLevel() const { return ownerLevel.lock(); }
		inline std::shared_ptr<Widget> GetOwnerWidget() const { return ownerWidget.lock(); }
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }
		inline const Vector2Int& GetPosition() const { return position; }
		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }
		inline int GetRenderSortingOrder() const { return renderSortingOrder; }

	protected:
		/* 자식 위젯 추가 함수 */
		template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<Widget, T>::value>>
		std::shared_ptr<T> AddChildWidget(const Vector2Int& positionLT, Args&& ...args)
		{
			std::shared_ptr<T> createdWidget = std::make_shared<T>(GetPosition() + positionLT, std::forward<Args>(args)...);

			createdWidget->SetOwnerWidget(weak_from_this());

			widgetTree.emplace_back(createdWidget);

			return createdWidget;
		}

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

		// OwnerLevel - 위젯을 소유하는 레벨 객체
		std::weak_ptr<Level> ownerLevel;

		// Owner - 이 위젯의 부모 위젯
		std::weak_ptr<Widget> ownerWidget;

		// Widget 내부 계층 구조
		std::vector<std::shared_ptr<Widget>> widgetTree;
	};
}



#include "Widget.h"

namespace Craft
{
	Widget::Widget(const Vector2Int& positionLT, int widgetWidth, int widgetHeight)
		:position(positionLT)
		,width(widgetWidth)
		,height(widgetHeight)
	{

	}

	Widget::~Widget()
	{

	}

	void Widget::BeginPlay()
	{
		if (hasBeganPlay)
		{
			return;
		}

		//자식 위젯들의 BeginPlay 호출
		for (std::shared_ptr<Widget>& childWidget : widgetTree)
		{
			if (!childWidget || childWidget->HasBeganPlay())
			{
				continue;
			}

			childWidget->BeginPlay();
		}

		// 중복 호출 방지를 위해 설정.
		hasBeganPlay = true;
	}

	void Widget::Tick(float deltaTime)
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		//자식 위젯들 Tick 호출
		for (std::shared_ptr<Widget>& childWidget : widgetTree)
		{
			if (!childWidget || !childWidget->IsActive())
			{
				continue;
			}

			childWidget->Tick(deltaTime);
		}
	}

	void Widget::Draw()
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		//자식 위젯들 Draw 호출
		for (std::shared_ptr<Widget>& childWidget : widgetTree)
		{
			if (!childWidget || !childWidget->IsActive())
			{
				continue;
			}

			childWidget->Draw();
		}
	}

	void Widget::Destroy()
	{
		//자식 위젯들의 Destory 이벤트 호출 
		for (std::shared_ptr<Widget>& childWidget : widgetTree)
		{
			if (!childWidget)
			{
				continue;
			}

			childWidget->Destroy();
		}

		// 삭제 예약
		// 다음 프레임에 위젯이 레벨에서 제거됨.
		hasExpired = true;
	}

	void Widget::SetActive(bool bActive)
	{
		isActive = bActive;
	}

	void Widget::SetRenderSortingOrder(int order)
	{
		renderSortingOrder = order;

		//자식 위젯들 Owner 설정
		for (std::shared_ptr<Widget>& childWidget : widgetTree)
		{
			if (!childWidget)
			{
				continue;
			}

			childWidget->SetRenderSortingOrder(order);
		}
	}

	void Widget::SetOwnerLevel(std::weak_ptr<Level> newOwner)
	{
		ownerLevel = newOwner;

		//자식 위젯들 Owner 설정
		for (std::shared_ptr<Widget>& childWidget : widgetTree)
		{
			if (!childWidget)
			{
				continue;
			}

			childWidget->SetOwnerLevel(newOwner);
		}
	}

	void Widget::SetOwnerWidget(std::weak_ptr<Widget> newOwner)
	{
		ownerWidget = newOwner;
	}
}

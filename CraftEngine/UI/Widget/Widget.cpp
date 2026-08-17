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
	}

	void Widget::Draw()
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}
	}

	void Widget::Destroy()
	{
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
	}

	void Widget::SetOwner(std::weak_ptr<Level> newOwner)
	{
		owner = newOwner;
	}
}

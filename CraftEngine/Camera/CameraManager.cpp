#include "CameraManager.h"

namespace Craft
{
	CameraManager::CameraManager(int width, int height, int spaceMaxWidth, int spaceMaxHeight)
		:viewWidth(width)
		,viewHeight(height)
	{
		const int halfViewWidth = viewWidth >> 1;
		const int halfViewHeight = viewHeight >> 1;

		limitLeftTopPos.x = halfViewWidth;
		limitLeftTopPos.y = halfViewHeight;

		limitRightDownPos.x = (spaceMaxWidth - viewWidth) + halfViewWidth;
		limitRightDownPos.y = (spaceMaxHeight - viewHeight) + halfViewHeight;

		SetViewPosition(limitLeftTopPos);
	}

	CameraManager::~CameraManager()
	{

	}

	void CameraManager::SetViewPosition(const Vector2Int& position)
	{
		/*if (position.x < limitLeftTopPos.x || position.x > limitRightDownPos.x ||
			position.y < limitLeftTopPos.y || position.y > limitRightDownPos.y)
		{
			int a = 10;
			a = a;
		}*/

		viewPosition.x = min(max(position.x, limitLeftTopPos.x), limitRightDownPos.x);
		viewPosition.y = min(max(position.y, limitLeftTopPos.y), limitRightDownPos.y);
	}

	void CameraManager::SetViewWidthHeight(int width, int height)
	{
		viewWidth = width;
		viewHeight = height;
	}

	Vector2Int CameraManager::GetLeftTopPosition() const
	{
		return Vector2Int(viewPosition.x - (viewWidth >> 1), viewPosition.y - (viewHeight >> 1));
	}

	Vector2Int CameraManager::GetRightDownPosition() const
	{
		return GetLeftTopPosition() + Vector2Int(viewWidth, viewHeight);
	}
}


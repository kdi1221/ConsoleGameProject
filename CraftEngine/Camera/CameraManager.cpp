#include "CameraManager.h"

namespace Craft
{
	CameraManager::CameraManager(int width, int height)
		:viewWidth(width)
		,viewHeight(height)
	{

	}

	CameraManager::~CameraManager()
	{

	}

	void CameraManager::SetViewPosition(const Vector2Int& position)
	{
		viewPosition.x = min(max(position.x, limitLeftTopPos.x), limitRightDownPos.x);
		viewPosition.y = min(max(position.y, limitLeftTopPos.y), limitRightDownPos.y);
	}

	void CameraManager::SetViewWidthHeight(int width, int height)
	{
		viewWidth = width;
		viewHeight = height;
	}

	void CameraManager::SetLimitPosition(const Vector2Int& leftTopPos, const Vector2Int& rightDownPos)
	{
		limitLeftTopPos = leftTopPos;
		limitRightDownPos = rightDownPos;
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


#pragma once

#include "Core/Core.h"
#include "Math/Vector2Int.h"

namespace Craft
{
	/* 게임 화면 표시 카메라 관리 클래스 */
	class CRAFT_API CameraManager
	{
	public:
		CameraManager(int width, int height, int spaceMaxWidth, int spaceMaxHeight);
		~CameraManager();

	public:
		void SetViewPosition(const Vector2Int& position);
		void SetViewWidthHeight(int width, int height);

	public:
		Vector2Int GetLeftTopPosition() const;
		Vector2Int GetRightDownPosition() const;

	public:
		inline const Vector2Int& GetViewPosition() const { return viewPosition; }
		inline int GetViewWidth() const { return viewWidth; }
		inline int GetViewHeight() const { return viewHeight; }

	private:
		/* 뷰 위치(카메라의 위치)*/
		Vector2Int viewPosition = Vector2Int::Zero;

		/* 뷰포트 가로 길이 */
		int viewWidth = 0;

		/* 뷰포트 세로 길이 */
		int viewHeight = 0;

		/* Left Top 최소 위치 */
		Vector2Int limitLeftTopPos = Vector2Int::Zero;

		/* Right Bottom 최대 위치 */
		Vector2Int limitRightDownPos = Vector2Int::Zero;
	};
}



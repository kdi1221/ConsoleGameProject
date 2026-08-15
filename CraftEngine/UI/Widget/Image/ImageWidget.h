#pragma once

#include "UI/Widget/Widget.h"
#include <string>
#include "Math/Color.h"

namespace Craft
{
	/* 이미지 표시 위젯(콘솔 엔진에서는 텍스트블록과 큰차이없음) */
	class CRAFT_API ImageWidget : public Widget
	{
		TYPE_DECLARATIONS(ImageWidget, Widget)
	
	public:
		ImageWidget(const Vector2Int& positionLT, const std::wstring& newImage, const Color newColor = Color::White);
		virtual ~ImageWidget() = default;

	protected:
		virtual void Draw() override;

	private:
		std::wstring imageValue = L"";
		Color drawColor = Color::White;
	};
}



#pragma once

#include "UI/Widget/Widget.h"
#include <string>
#include "Math/Color.h"

namespace Craft
{
	/* 텍스트 한줄 표시하는 위젯 */
	class CRAFT_API TextBlock : public Widget
	{
		TYPE_DECLARATIONS(TextBlock, Widget)

	public:
		TextBlock(const Vector2Int& positionLT, int widgetWidth);
		virtual ~TextBlock() = default;

	protected:
		virtual void Draw() override;

	public:
		void SetTextValue(const std::wstring& text);
		void SetDrawColor(const Color newColor);

	public:
		inline const std::wstring& GetTextValue() const { return textValue; }
		inline Color GetDrawColor() const { return drawColor; }

	private:
		std::wstring textValue = L"";
		Color drawColor = Color::White;
	};
}




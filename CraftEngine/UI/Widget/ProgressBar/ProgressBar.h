#pragma once

#include "UI/Widget/Widget.h"
#include "Math/Color.h"

namespace Craft
{
	/* 프로그래스바 */
	class CRAFT_API ProgressBar : public Widget
	{
		TYPE_DECLARATIONS(ProgressBar, Widget)

	public:
		ProgressBar(const Vector2Int& positionLT, 
					const int drawWidth, 
					const Color background = Color::White, 
					const Color foreground = Color::Blue);
		virtual ~ProgressBar() = default;

	public:
		void SetValue(float inCurrentValue, float inMaxValue);

	protected:
		virtual void Draw() override;

	private:
		Color backgroundColor = Color::White;
		Color foregroundColor = Color::Blue;

		float currentValue = 0.0f;
		float maxValue = 1.f;

		int drawCurrentValueNum = 0;
		int drawMaxValueNum = 0;
	};
}


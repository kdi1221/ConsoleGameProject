#pragma once

#include "Component.h"
#include "Math/Vector2Int.h"
#include "Math/Color.h"
#include <string>
#include <vector>

namespace Craft
{
	//여러 줄을 차지하는 글자(이미지)를 그리는 기능을 담당
	class CRAFT_API MultiSpriteRendererComponent : public Component
	{
		TYPE_DECLARATIONS(MultiSpriteRendererComponent, Component)

	private:
		struct FSpriteInfo
		{
			/* 중심 좌표 기준 offset */
			Vector2Int imageOffset = Vector2Int::Zero;

			/* 화면에 출력할 문자열 */
			std::wstring image = L"";

			FSpriteInfo() = default;
			FSpriteInfo(const Vector2Int& offset, const std::wstring& image)
				:imageOffset(offset)
				,image(image)
			{

			}
		};

	public:
		MultiSpriteRendererComponent(Color color = Color::White, int sortingOrder = 0);
		virtual ~MultiSpriteRendererComponent() = default;

	public:
		virtual void Draw() override;

	public:
		void AppendImage(const Vector2Int& offset, const std::wstring& image);

	public:
		inline Color GetColor() const { return color; }
		inline void SetColor(Color newColor) { color = newColor; }

		inline int GetSortingOrder() const { return sortingOrder; }
		inline void SetSortingOrder(int newSortingOrder) { sortingOrder = newSortingOrder; }

	protected:
		//글자 색상 값
		Color color = Color::White;

		//같은 위치에 여러 문자가 있을 때 그릴 우선순위.
		int sortingOrder = 0;

		/* 화면에 출력할 문자열들(이미지) */
		std::vector<FSpriteInfo> drawImages;
	};
}



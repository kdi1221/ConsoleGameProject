#pragma once

#include "Component.h"
#include "Math/Color.h"
#include <string>

namespace Craft
{
	//글자(이미지)를 그리는 기능을 담당하는 컴포넌트.
	class CRAFT_API SpriteRendererComponent : public Component
	{
		TYPE_DECLARATIONS(SpriteRendererComponent, Component)

	public:
		SpriteRendererComponent(const std::string& image = "", Color color = Color::White, int sortingOrder = 0);
		virtual ~SpriteRendererComponent() = default;

	public:
		virtual void Draw() override;

		//Getter/Setter
		inline const std::string& GetImage() const { return image; }
		inline void SetImage(const std::string& newImage) { image = newImage; }
		inline int GetWidth() const { return static_cast<int>(image.size()); }

		inline Color GetColor() const { return color; }
		inline void SetColor(Color newColor) { color = newColor; }

		inline int GetSortingOrder() const { return sortingOrder; }
		inline void SetSortingOrder(int newSortingOrder) { sortingOrder = newSortingOrder; }

	protected:
		//화면에 출력할 문자열.
		std::string image;
		
		//글자 색상 값.
		Color color = Color::White;

		//같은 위치에 여러 문자가 있을 때 그릴 우선순위.
		int sortingOrder = 0;

	};
}




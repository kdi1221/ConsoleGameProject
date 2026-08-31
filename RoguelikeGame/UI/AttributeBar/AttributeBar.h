#pragma once

#include <UI/Widget/Widget.h>
#include <Math/Color.h>
#include <unordered_map>
#include <string>

/* 체력, 마나 등의 Attribute 원형 표시 위젯 */
class AttributeBar : public Craft::Widget
{
	TYPE_DECLARATIONS(AttributeBar, Widget)

public:
	AttributeBar(const Craft::Vector2Int& positionLT, int radius, Craft::Color innerColor, Craft::Color outerColor);
	virtual ~AttributeBar();

private:
	virtual void Draw() override;

public:
	void SetValue(float amout, float maxAmount);

private:
	//외부 영역 색상
	Craft::Color outerFillColor = Craft::Color::White;

	//내부 영역 색상
	Craft::Color innerFillColor = Craft::Color::White;

	//외부 영역 위치들
	std::vector<Craft::Vector2Int> outerPoints;

	//내부 영역을 채울 이미지(문자열)들
	std::unordered_map<Craft::Vector2Int, std::wstring> innerFillImages;

	//y 최소값(위)
	int yMinPos = 0;

	//y 최대값(밑)
	int yMaxPos = 0;

	//바 최대 높이값
	int barMaxHeight = 0;

	//현재 값
	float currentAmount = 0.f;

	//최대 값
	float maxValue = 0.f;

	//내부 영역 출력시 실제 출력되어야 될 y 시작 위치
	int drawYOffsetPos = 0;
};


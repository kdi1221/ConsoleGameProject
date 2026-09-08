#include "AttributeBar.h"
#include <Render/Renderer.h>
#include <StaticLibrary/StaticFunctionLibrary.h>

using namespace Craft;

AttributeBar::AttributeBar(const Vector2Int& positionLT, int radius, Craft::Color innerColor, Craft::Color outerColor, Craft::Color amountColor)
	:super(positionLT, radius, radius)
	,innerFillColor(innerColor)
	,outerFillColor(outerColor)
	,amountTextColor(amountColor)
{
	//브레젠험 원 그리기로 원 영역 구하기
	//const Vector2Int centerPos = positionLT + Vector2Int(radius, radius);

	/* y축 별 x 범위 */
	std::unordered_map<int, std::pair<int, int>> mapCircleXRange;

	/* y 최소,최대 값 */
	int xMin = INT_MAX, xMax = INT_MIN;
	int yMin = INT_MAX, yMax = INT_MIN;

	auto callbackCirclePoints = [&](int xPos, int yPos)
		{
			/* 외곽 영역 포인트 저장 */
			//outerPoints.emplace_back(centerPos + Vector2Int(xPos, yPos));
			outerPoints.emplace_back(Vector2Int(xPos, yPos));

			/* 원 내부 영역을 구하기위한 x범위를 y위치별로 저장 */
			auto iterYPosXRange = mapCircleXRange.find(yPos);
			if (iterYPosXRange == mapCircleXRange.end())
			{
				mapCircleXRange.insert({ yPos, {xPos, xPos} });
			}
			else
			{
				iterYPosXRange->second.first = min(iterYPosXRange->second.first, xPos);
				iterYPosXRange->second.second = max(iterYPosXRange->second.second, xPos);
			}

			/* X값의 min, max 갱신 */
			xMin = min(xMin, xPos);
			xMax = max(xMax, xPos);

			/* Y값의 min, max 갱신 */
			yMin = min(yMin, yPos);
			yMax = max(yMax, yPos);
		};

	//StaticFunctionLibrary::BresenhamCircleProcess(positionLT, radius, callbackCirclePoints, 1.f);
	StaticFunctionLibrary::MidpointCircleProcess(positionLT, radius, static_cast<int>(radius * 0.6f), callbackCirclePoints);

	/* y축별로 x min ~ x max를 돌아가면서 원 내부를 채운다. */
	for (auto& iterCircleXRange : mapCircleXRange)
	{
		const int yPos = iterCircleXRange.first;
		if (yPos == yMin || yPos == yMax)
		{
			continue;
		}

		const int xStartPos = iterCircleXRange.second.first + 1;
		const int xEndPos = iterCircleXRange.second.second - 1;
		const int drawWidth = xEndPos - xStartPos;
		
		std::wstring innerFillImage;
		innerFillImage.resize(drawWidth + 1);
		for (int addX = 0; addX <= drawWidth; ++addX)
		{
			innerFillImage[addX] = L'█';
		}

		//innerFillImages.insert({ centerPos + Vector2Int(xStartPos, yPos), innerFillImage });
		innerFillImages.insert({ Vector2Int(xStartPos, yPos), innerFillImage });
	}

	//y 최소값(위)
	//yMinPos = centerPos.y + yMin;
	yMinPos = yMin;

	//y 최대값(밑)
	//yMaxPos = centerPos.y + yMax;
	yMaxPos = yMax;

	xMinPos = xMin;
	xMaxPos = xMax;

	centerPosition = Vector2Int((xMinPos + xMaxPos) >> 1, (yMinPos + yMaxPos) >> 1);
	drawStringAmountPos = centerPosition;

	//바 높이
	barMaxHeight = yMaxPos - yMinPos;

	//YOffset은 y위치로 초기화
	drawYOffsetPos = positionLT.y;
}

AttributeBar::~AttributeBar() = default;

void AttributeBar::Draw() 
{
	super::Draw();

	Renderer& renderer = Renderer::Get();
	const int renderSortingOrder = GetRenderSortingOrder();

	/* 내부 영역을 먼저 그리기 */
	for (const auto& iterInnerImage : innerFillImages)
	{
		const Vector2Int& drawPos = iterInnerImage.first;

		/* y위치가 현재 그리기가 시작되어야 될 YOffset보다 아래에 있을때만 출력한다. */
		if (drawPos.y < drawYOffsetPos)
		{
			continue;
		}

		const std::wstring& drawInnerImage = iterInnerImage.second;

		if (drawInnerImage.empty())
		{
			continue;
		}

		renderer.SubmitUI(drawInnerImage, drawPos, innerFillColor, renderSortingOrder);
	}

	/* 외부 영역을 위에 덮어씌움 */
	for (const Vector2Int& outerPostion : outerPoints)
	{
		renderer.SubmitUI(L"█", outerPostion, outerFillColor, renderSortingOrder);
	}
	
	/* 현재/최대 값 표시 */
	renderer.SubmitUI(strAmount, drawStringAmountPos, amountTextColor, renderSortingOrder);
}

void AttributeBar::SetValue(float amout, float maxAmount)
{
	maxValue = maxAmount;
	currentAmount = min(amout, maxValue);
	
	const float percentage = amout / maxValue;

	//바 현재 높이값
	const int barYOffset = barMaxHeight - static_cast<int>(static_cast<float>(barMaxHeight) * percentage);
	//drawYOffsetPos = GetPosition().y + barYOffset;
	drawYOffsetPos = yMinPos + barYOffset;

	strAmount = std::to_wstring(static_cast<int>(currentAmount)) + L"/" + std::to_wstring(static_cast<int>(maxValue));
	const int strAmountLength = static_cast<int>(strAmount.length());
	drawStringAmountPos = centerPosition + Vector2Int(-(strAmountLength >> 1), 0);
}

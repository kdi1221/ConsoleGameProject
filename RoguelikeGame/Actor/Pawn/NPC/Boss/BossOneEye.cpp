#include "BossOneEye.h"
#include "Actor/Pawn/Pawn.h"
#include "Resource/ResourceBossEye.h"
#include "Types/Enums.h"
#include <Util/Util.h>
#include <Render/Renderer.h>
#include <Resource/ResourceManager.h>
#include <Engine/Engine.h>

using namespace Craft;

BossOneEye::BossOneEye(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, 5000.f, eMonsterPattern::Boss, roomIndex)
{
	timerAwakeDelay.SetTargetTime(5.f);
}

BossOneEye::~BossOneEye()
{

}

void BossOneEye::Initialize()
{
	super::Initialize();

	const ResourceManager& resourceManager = Engine::Get().GetResourceManager<ResourceManager>();
	resourceBossEye = Cast<ResourceBossEye>(resourceManager.FindGameResource(ResourceManager::eResourceCategory::Image, 1));
	assert(resourceBossEye && "Invalid resourceBossEye..");
}

void BossOneEye::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!bAwake)
	{
		timerAwakeDelay.Tick(deltaTime);
		if (timerAwakeDelay.IsTimeOut())
		{
			StartOpenEye();
			bAwake = true;
		}
	}

	if (bChaseTarget)
	{
		UpdateChaseTargetOffset(deltaTime);
		LerpCurrentOffset(deltaTime);
	}

	if (eEyeOpenAnimation::None != currentEyeOpenAnimation)
	{
		LerpOpenCloseAnimation(deltaTime);
	}
}

void BossOneEye::Draw()
{
	super::Draw();

	if (!resourceBossEye)
	{
		return;
	}

	Renderer& renderer = Renderer::Get();
		
	/* 각 line별 eye Image 그리기 */
	auto drawEyeImage = [&renderer](const Vector2Int& centerPos, const ResourceBossEye::FEyeImage& eyeImage, const Vector2Int& offset = Vector2Int::Zero)
		{
			for (const ResourceBossEye::FEyeLineImage& lineImage : eyeImage.lineImages)
			{
				const Vector2Int drawLinePos = centerPos + lineImage.centerOffset + offset;
		
				renderer.Submit(lineImage.image, drawLinePos, eyeImage.drawColor, static_cast<int>(eRenderSortingOrder::Boss));
			}
		};

	const Vector2Int& centerPos = GetWorldPosition();

	const ResourceBossEye::FEyeImage& outlineImage = resourceBossEye->GetEyeImage(ResourceBossEye::eEyeImageCategory::Outline);

	/* line이 짝수면 끝 offset을 0으로, 홀수면 끝 offset을 1로 둔다. */
	const int lineImageNum = static_cast<int>(outlineImage.lineImages.size());
	const int endLineOffset = static_cast<int>(lineImageNum % 2);

	if (0 == currentEyeClosureOffset)
	{
		/* 외곽라인 그리기(맨 뒤) */
		drawEyeImage(centerPos, outlineImage);
	}
	else
	{
		//눈이 감긴 동안에는 맨처음과 끝은 항상 Draw,
		Vector2Int drawLinePos = centerPos + outlineImage.lineImages[0].centerOffset;
		renderer.Submit(outlineImage.lineImages[0].image, drawLinePos, outlineImage.drawColor, static_cast<int>(eRenderSortingOrder::Boss));
		drawLinePos = centerPos + outlineImage.lineImages[lineImageNum - 1].centerOffset;
		renderer.Submit(outlineImage.lineImages[lineImageNum - 1].image, drawLinePos, outlineImage.drawColor, static_cast<int>(eRenderSortingOrder::Boss));

		//나머진 현재 offset을 제외하고 그려줘야 한다.
		const int drawStartIndex = currentEyeClosureOffset + 1;
		const int drawEndIndex = drawStartIndex + lineImageNum - 2 - (currentEyeClosureOffset << 1) + endLineOffset;
		for (int drawLineIndex = drawStartIndex; 
			drawLineIndex < drawEndIndex;
			++drawLineIndex)
		{
			drawLinePos = centerPos + outlineImage.lineImages[drawLineIndex].centerOffset;
			renderer.Submit(outlineImage.lineImages[drawLineIndex].image, drawLinePos, outlineImage.drawColor, static_cast<int>(eRenderSortingOrder::Boss));
		}
	}
		
	/* 공막(눈 흰자) */
	drawEyeImage(centerPos, resourceBossEye->GetEyeImage(ResourceBossEye::eEyeImageCategory::Sclera));

	/* 홍채 */
	drawEyeImage(centerPos, resourceBossEye->GetEyeImage(ResourceBossEye::eEyeImageCategory::Iris), drawIrisPupilOffset);

	/* 동공 */
	drawEyeImage(centerPos, resourceBossEye->GetEyeImage(ResourceBossEye::eEyeImageCategory::Pupil), drawIrisPupilOffset);

	//눈이 감긴 동안에는 offset부분을 위아래로 그려줘야 한다. 
	if (0 != currentEyeClosureOffset)
	{
		for (int drawLineNum = 1; drawLineNum <= currentEyeClosureOffset; ++drawLineNum)
		{
			/* 위쪽 아웃라인 그리기 */
			const int upOutlineIndex = drawLineNum;

			Vector2Int drawLinePos = centerPos + outlineImage.lineImages[upOutlineIndex].centerOffset;
			renderer.Submit(outlineImage.lineImages[upOutlineIndex].image, drawLinePos, outlineImage.drawColor, static_cast<int>(eRenderSortingOrder::Boss));

			/* 아래쪽 아웃라인 그리기 */
			const int downOutlineIndex = lineImageNum - 1 - drawLineNum;

			/* 위쪽 아웃라인이 아래쪽 인덱스를 넘으면 더이상 그릴 필요가 없으니 break */
			if (upOutlineIndex >= downOutlineIndex)
			{
				break;
			}

			drawLinePos = centerPos + outlineImage.lineImages[downOutlineIndex].centerOffset;
			renderer.Submit(outlineImage.lineImages[downOutlineIndex].image, drawLinePos, outlineImage.drawColor, static_cast<int>(eRenderSortingOrder::Boss));
		}
	}
}

void BossOneEye::ForEachOccupiedTileOffset(std::function<void(const Craft::Vector2Int&)> callbackFunc) const
{
	if (!resourceBossEye)
	{
		return;
	}

	/* 외곽 라인의 각 라인별로 offset들을 계산하여 점유 타일 오프셋으로 넘겨준다. */
	const ResourceBossEye::FEyeImage& outlineImage = resourceBossEye->GetEyeImage(ResourceBossEye::eEyeImageCategory::Outline);
	for (const ResourceBossEye::FEyeLineImage& lineImage : outlineImage.lineImages)
	{
		const int imageLength = static_cast<int>(lineImage.image.length());
		for (int xAdd = 0; xAdd < imageLength; ++xAdd)
		{
			callbackFunc(lineImage.centerOffset + Vector2Int(xAdd, 0));
		}
	}
}

void BossOneEye::UpdateChaseTargetOffset(float deltaTime)
{
	/* 새로 지정할 타겟을 향한 오프셋 */
	Vector2Float newTargetOffset = Vector2Float::Zero;

	std::shared_ptr<Pawn> chaseTargetPtr = GetChaseTarget();
	if (chaseTargetPtr && !chaseTargetPtr->HasExpired() && !chaseTargetPtr->IsDeath())
	{
		/* 중심 위치 */
		const Vector2Int& centerPos = GetWorldPosition();

		/* 추적 타겟의 위치 */
		const Vector2Int& targetPos = chaseTargetPtr->GetWorldPosition();

		/* 중심위치로부터 추적타겟의 방향 */
		Vector2Float toTargetDistanceFloat = static_cast<Vector2Float>(targetPos - centerPos);

		/* 눈 타원의 가로, 세로 반지름 */
		const float xRadius = 12.f, yRadius = 3.f;
		//const float xRadius = 8.f, yRadius = 1.f;

		/* x의 제곱 / 가로 반지름의 제곱 */
		const float xRatio = (toTargetDistanceFloat.x * toTargetDistanceFloat.x) / (xRadius * xRadius);

		/* y의 제곱 / 세로 반지름의 제곱 */
		const float yRatio = (toTargetDistanceFloat.y * toTargetDistanceFloat.y) / (yRadius * yRadius);

		/* 각 축의 제곱의 합 */
		const float ratioSum = xRatio + yRatio;

		/* 제곱의 합이 1을 넘어서면 타원의 범위를 벗어난 것 */
		if (ratioSum > 1.f)
		{
			/* 타원의 범위(1) 안에서 xDistance와 yDistance를 다시 계산한다. */

			/* scale의 제곱 * (x의 제곱 / 가로 반지름의 제곱 + y의 제곱 / 세로 반지름의 제곱) = 1 */
			/* scale의 제곱 = 1 / (x의 제곱 / 가로 반지름의 제곱 + y의 제곱 / 세로 반지름의 제곱) */
			/* scale = sqrt(1 / (x의 제곱 / 가로 반지름의 제곱 + y의 제곱 / 세로 반지름의 제곱)) */
			const float innerScale = sqrt(1.f / ratioSum);

			toTargetDistanceFloat.x *= innerScale;
			toTargetDistanceFloat.y *= innerScale;
		}

		/* 선형보간 타겟 offset 대입 */
		newTargetOffset = toTargetDistanceFloat;
	}
	
	/* 현재 타겟 오프셋과 다른경우에는 경과 시간 초기화 */
	if (newTargetOffset != targetIrisPupilOffset)
	{
		targetIrisPupilOffset = newTargetOffset;

		/* 애니메이션 경과시간 초기화 */
		elapsedtimeIrisPupilAnimation = 0.f;
	}	
}

void BossOneEye::LerpCurrentOffset(float deltaTime)
{
	if (currentIrisPupilOffset == targetIrisPupilOffset)
	{
		return;
	}

	/* 애니메이션 경과 시간 업데이트 */
	elapsedtimeIrisPupilAnimation += deltaTime;
	elapsedtimeIrisPupilAnimation = min(elapsedtimeIrisPupilAnimation, durationIrisPupilAnimation);

	/* 선형 보간*/
	currentIrisPupilOffset = Vector2Float::Lerp(currentIrisPupilOffset,
												targetIrisPupilOffset, 
												elapsedtimeIrisPupilAnimation / durationIrisPupilAnimation);

	/* 반올림해서 정수형 offset 계산 */
	drawIrisPupilOffset.x = static_cast<int>(round(currentIrisPupilOffset.x));
	drawIrisPupilOffset.y = static_cast<int>(round(currentIrisPupilOffset.y));
}

void BossOneEye::LerpOpenCloseAnimation(float deltaTime)
{
	if (currentEyeOpenAnimation == eEyeOpenAnimation::None)
	{
		return;
	}

	elapsedtimeOpenCloseAnimation += deltaTime;
	elapsedtimeOpenCloseAnimation = min(elapsedtimeOpenCloseAnimation, durationOpenCloseAnimation);

	const float startLerpValue = currentEyeOpenAnimation == eEyeOpenAnimation::OpenEye ? static_cast<float>(maxEyeClosure) : 0.f;
	const float endLerpValue = currentEyeOpenAnimation == eEyeOpenAnimation::OpenEye ? 0.f : static_cast<float>(maxEyeClosure);

	float lerpEyeClosure = Util::Lerp(startLerpValue, endLerpValue, elapsedtimeOpenCloseAnimation / durationOpenCloseAnimation);
	currentEyeClosureOffset = static_cast<int>(round(lerpEyeClosure));

	if (elapsedtimeOpenCloseAnimation >= durationOpenCloseAnimation)
	{
		OnFinishEyeOpenCloseAnimation(currentEyeOpenAnimation);

		currentEyeOpenAnimation = eEyeOpenAnimation::None;
		elapsedtimeOpenCloseAnimation = 0.f;
	}
}

void BossOneEye::StartOpenEye()
{
	currentEyeOpenAnimation = eEyeOpenAnimation::OpenEye;
	elapsedtimeOpenCloseAnimation = 0.f;
}

void BossOneEye::StartCloseEye()
{
	currentEyeOpenAnimation = eEyeOpenAnimation::CloseEye;
	elapsedtimeOpenCloseAnimation = 0.f;
}

void BossOneEye::OnFinishEyeOpenCloseAnimation(eEyeOpenAnimation prevAnimation)
{
	if (prevAnimation == eEyeOpenAnimation::OpenEye)
	{
		/* 최초 생성후 눈을 뜬 이후에 타겟을 추적 */
		if (!bChaseTarget)
		{
			bChaseTarget = true;
		}	
	}
}

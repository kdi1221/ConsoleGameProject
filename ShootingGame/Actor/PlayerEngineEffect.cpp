#include "PlayerEngineEffect.h"
#include "Component/SpriteRendererComponent.h"

//엔진 이펙트에 사용할 애니메이션 프레임
static const wchar_t* engineEffectFrames[] = {L" * ", L"***" };

using namespace Craft;

PlayerEngineEffect::PlayerEngineEffect(const Vector2Float& localPosition)
	:super(localPosition)
{
	//컴포넌트 추가.
	spriteRendererComponent = AddComponent<SpriteRendererComponent>(engineEffectFrames[0], Color::Red, 4);
}

void PlayerEngineEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//타이머 설정 및 애니메이션 재생.
	elapsedTime += deltaTime;
	if (elapsedTime < effectFrameTime)
	{
		return;
	}

	//충분한 시간이 흘렀으면 타이머 초기화
	elapsedTime = 0.f;

	//재생할 다음 인덱스 구하기.
	const int frameCount = sizeof(engineEffectFrames) / sizeof(engineEffectFrames[0]);
	frameIndex = (frameIndex + 1) % frameCount;

	//이미지(문자열) 설정
	if (spriteRendererComponent)
	{
		spriteRendererComponent->SetImage(engineEffectFrames[frameIndex]);
	}
}

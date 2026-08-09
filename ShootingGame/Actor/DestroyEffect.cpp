#include "DestroyEffect.h"
#include "Engine/Engine.h"
#include "Component/SpriteRendererComponent.h"
#include "Engine/Config/ConfigBase.h"

using namespace Craft;
using EffectFrame = DestroyEffect::EffectFrame;

// 효과 재생에 사용할 이펙트 시퀀스(일종의 애니메이션 프레임 시퀀스)
static const EffectFrame sequence[] =
{
	{ L"  @  ", 0.08f, Color::Red },
	{ L" @@  ", 0.08f, Color::Blue },
	{ L" @@@ ", 0.08f, Color::Green },
	{ L"@@@@ ", 0.08f, Color::Blue },
	{ L"@@@@@", 0.5f, Color::Red },
};

DestroyEffect::DestroyEffect(const Craft::Vector2Float& position)
	:super(position)
{
	//컴포넌트 추가
	spriteRendererComponent = AddComponent<SpriteRendererComponent>(sequence[0].frame, sequence[0].color, 7);

	//시퀀스에 사용할 문자열 길이 값.
	int effectFrameImageLength = static_cast<int>(sequence[0].frame.size());

	//화면 위치.
	Vector2Float tempPosition = GetPosition();

	// x위치 조정.
	tempPosition.x = tempPosition.x < 0 ? effectFrameImageLength + tempPosition.x : tempPosition.x;
	tempPosition.x = tempPosition.x + effectFrameImageLength > Engine::Get().GetConfig<ConfigBase>().GetDisplayWidth() ? tempPosition.x - effectFrameImageLength : tempPosition.x;

	// 보정된 위치 설정
	SetPosition(tempPosition);

	// 이펙트 시퀀스 배열의 길이.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// 타이머 설정(다음 이펙트 프레임시간까지 대기).
	timer.SetTargetTime(sequence[0].playTime);

	//색상 설정
	//color = sequence[0].color;

	// 현재 재생 중인 시퀀스 인덱스 설정.
	currentSequenceIndex = 0;
}

void DestroyEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	//시퀀스를 모두 재생했는지 확인. -> 삭제
	if (currentSequenceIndex == effectSequenceCount)
	{
		Destroy();
		return;
	}

	//타이머 리셋.
	timer.Reset();

	//이펙트 프레임 업데이트.
	++currentSequenceIndex;

	// 다음 프레임에서 재생할 시간으로 타이머 재설정.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	//새로운 프레임 이미지로 교체 및 색상 설정
	if (spriteRendererComponent)
	{
		spriteRendererComponent->SetImage(sequence[currentSequenceIndex].frame);
		spriteRendererComponent->SetColor(sequence[currentSequenceIndex].color);
	}
	
	//ChangeImage(sequence[currentSequenceIndex].frame);

	//색상 설정
	//color = sequence[currentSequenceIndex].color;

}

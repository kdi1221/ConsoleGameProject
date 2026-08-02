#pragma once

#include "Actor/Actor.h"
#include "Util/Util.h"
#include "Util/Timer.h"

namespace Craft
{
	class SpriteRendererComponent;
}

class DestroyEffect : public Craft::Actor
{
	TYPE_DECLARATIONS(DestroyEffect, Actor)

	//이펙트 프레임 구조체
	struct EffectFrame
	{
		EffectFrame(
			const std::string& frame,
			float playTime,
			Craft::Color color)
			:frame(frame)
			,playTime(playTime)
			,color(color)
		{

		}

		~EffectFrame() = default;

		// 문자열 변수(화면에 보여줄 문자열).
		std::string frame;

		//재생 시간.
		float playTime = 0.f;

		//색상.
		Craft::Color color = Craft::Color::White;
	};

public:
	DestroyEffect(const Craft::Vector2& position);
	~DestroyEffect() = default;

private:
	// 효과 재생을 위한 Tick 이벤트
	virtual void Tick(float deltaTime) override;

private:
	// 시퀀스 배열의 길이
	int effectSequenceCount = 0;

	// 현재 보여지는 시퀀스 인덱스.
	int currentSequenceIndex = 0;

	// 시퀀스 재생에 사용할 타이머.
	Timer timer;

	// 이미지 효과를 업데이트할 때 사용할 컴포넌트 참조 변수.
	std::shared_ptr<Craft::SpriteRendererComponent> spriteRendererComponent;
};


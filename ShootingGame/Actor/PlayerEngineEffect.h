#pragma once

#include "Actor/Actor.h"

//전방 선언.
namespace Craft
{
	class SpriteRendererComponent;
}

//플레이어 아래 붙어서 추진 효과를 보여주는 액터
class PlayerEngineEffect : public Craft::Actor
{
	TYPE_DECLARATIONS(PlayerEngineEffect, Actor)

public:
	PlayerEngineEffect(const Craft::Vector2Float& localPosition);
	~PlayerEngineEffect() = default;

private:
	virtual void Tick(float deltaTime) override;

private:
	//애니메이션 프레임 교체할 때 사용할 컴포넌트
	std::shared_ptr<Craft::SpriteRendererComponent> spriteRendererComponent;

	//애니메이션 처리를 위한 시간 계산 변수
	float elapsedTime = 0.f;

	//애니메이션의 한 프레임 재생 시간(단위 : 초)
	float effectFrameTime = 0.08f;

	//현재 표시 중인 애니메이션 프레임 인덱스.
	int frameIndex = 0;
};


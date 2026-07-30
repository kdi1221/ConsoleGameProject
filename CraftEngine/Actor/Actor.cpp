#include "Actor.h"
#include "Engine/Engine.h"
#include "Level/Level.h"

namespace Craft
{
	Actor::Actor()
	{

	}

	Actor::~Actor()
	{

	}

	void Actor::BeginPlay()
	{
		// 중복 호출 방지를 위해 설정.
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{

	}

	void Actor::Draw()
	{

	}

	void Actor::Destroy()
	{
		// 액터 삭제 예약
		// 다음 프레임에 액터가 레벨에서 제거됨.
		hasExpired = true;
	}

	void Actor::QuitGame()
	{
		// 엔진 종료 요청
		Engine::Get().Quit();
	}

	std::shared_ptr<Level> Actor::GetOwner()
	{
		return owner.lock();
	}

	void Actor::SetOwner(std::weak_ptr<Level> newOwner)
	{
		owner = newOwner;
	}
}
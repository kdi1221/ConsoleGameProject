#include "Actor.h"
#include <Engine/Engine.h>
#include "Level/Level.h"
#include "Render/Renderer.h"

namespace Craft
{
	Actor::Actor(const std::string& image, const Vector2& position, Color color)
		:image(image), position(position), color(color), width(static_cast<int>(image.length()))
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
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		// 렌더러에 그릴 데이터 설정
		Renderer::Get().Submit(image, position, color, sortingOrder);
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

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 변경하려는 위치 값이 현재 위치와 같은지 비교.
		if (position == newPosition)
		{
			return;
		}

		//위치 업데이트.
		position = newPosition;
	}
}
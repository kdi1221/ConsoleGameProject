#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <memory>
#include <string>

namespace Craft
{
	//전방선언.
	class Level;

	class CRAFT_API Actor
	{
	public:
		Actor(const std::string& image = "", 
			const Vector2& position = Vector2::Zero, 
			Color color = Color::White);
		virtual ~Actor();

	public:
		//프레임 이벤트 함수.
		void BeginPlay();
		virtual void Tick(float deltaTime);
		void Draw();

		// 액터를 레벨에서 제거할 때 사용할 함수.
		void Destroy();

		// 게임(엔진)종료 요청 함수
		void QuitGame();

		//Getter/Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		std::shared_ptr<Level> GetOwner();
		void SetOwner(std::weak_ptr<Level> newOwner);

		inline Vector2 GetPosition() const { return position; }
		void SetPosition(const Vector2& newPosition);

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그.
		bool isActive = true;

		// 액터 삭제 예약 설정 플래그.
		bool hasExpired = false;

		// Owner - 액터를 소유하는 레벨 객체
		std::weak_ptr<Level> owner;

		// 화면에 그릴 글자(이미지).
		std::string image;

		//글자 색상.
		Color color = Color::White;

		//글자 길이
		int width = 0;

		// 그리기 정렬 순서.
		int sortingOrder = 0;

		// 액터 위치
		Vector2 position;
	};
}



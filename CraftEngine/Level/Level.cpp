#include "Level.h"

namespace Craft
{
	Level::Level()
	{

	}

	Level::~Level()
	{

	}

	void Level::OnInitialized()
	{
		//레벨 초기화 처리 플래그 설정.
		hasInitialized = true;
	}

	void Level::BeginPlay()
	{
		// 액터 초기화시 한번만 호출되는 이벤트
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			//이미 BeginPlay가 호출된 액터는 건너뛰기.
			if (actor->HasBeganPlay())
			{
				continue;
			}

			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			//비활성화 액터 건너뛰기.
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Tick(deltaTime);
		}
	}

	void Level::Draw()
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			//비활성화 액터 건너뛰기.
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Draw();
		}
	}

	void Level::ProcessAddAndDestroyActors()
	{
		// 액터 제거 처리.
		for (auto iterator = actorList.begin();
			iterator != actorList.end();)
		{
			//제거 요청 여부 확인
			auto actor = *iterator;
			if (actor->HasExpired())
			{
				iterator = actorList.erase(iterator);
				continue;
			}

			++iterator;
		}

		// 액터 추가 처리
		if (addRequestedActorList.empty())
		{
			return;
		}
		
		//추가 요청 처리
		for (const std::shared_ptr<Actor>& actor : addRequestedActorList)
		{
			actorList.emplace_back(actor);
		}
		addRequestedActorList.clear();
	}

	void Level::SavePreviousActorStates()
	{
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			actor->SavePreviousState();
		}
	}
}
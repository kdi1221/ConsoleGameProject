#include "Level.h"
#include "Actor/Actor.h"
#include <cassert>

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

		for (std::shared_ptr<Widget>& widget : widgetList)
		{
			//이미 BeginPlay가 호출된 Widget 건너뛰기
			if (widget->HasBeganPlay())
			{
				continue;
			}

			widget->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		if (!bGamePaused)
		{
			for (std::shared_ptr<Actor>& actor : actorList)
			{
				//비활성화 액터 건너뛰기.
				if (!actor->IsActive())
				{
					continue;
				}

				actor->PreTick(deltaTime);
			}

			for (std::shared_ptr<Actor>& actor : actorList)
			{
				//비활성화 액터 건너뛰기.
				if (!actor->IsActive())
				{
					continue;
				}

				actor->Tick(deltaTime);
			}

			for (std::shared_ptr<Actor>& actor : actorList)
			{
				//비활성화 액터 건너뛰기.
				if (!actor->IsActive())
				{
					continue;
				}

				actor->PostTick(deltaTime);
			}
		}
		
		for (std::shared_ptr<Widget>& widget : widgetList)
		{
			//비활성화 위젯 건너뛰기
			if (!widget->IsActive())
			{
				continue;
			}

			widget->Tick(deltaTime);
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

		for (std::shared_ptr<Widget>& widget : widgetList)
		{
			//비활성화 위젯 건너뛰기
			if (!widget->IsActive())
			{
				continue;
			}

			widget->Draw();
		}
	}

	void Level::SetGamePause(bool bPause)
	{
		bGamePaused = bPause;

		if (onGamePause)
		{
			onGamePause(bGamePaused);
		}
	}

	void Level::SetOnGamePause(OnGamePauseType callback)
	{
		onGamePause = callback;
	}

	void Level::ProcessAddAndDestroyActors()
	{
		//액터의 컴포넌트 추가 처리 함수 호출.
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			if (actor)
			{
				actor->ProcessAddComponents();
			}
		}

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
		if (!addRequestedActorList.empty())
		{
			//추가 요청 처리
			for (const std::shared_ptr<Actor>& actor : addRequestedActorList)
			{
				actorList.emplace_back(actor);
			}
			addRequestedActorList.clear();
		}

		//위젯 제거 처리
		for(auto iterator = widgetList.begin();
			iterator != widgetList.end();)
		{
			//제거 요청 여부 확인
			auto actor = *iterator;
			if (actor->HasExpired())
			{
				iterator = widgetList.erase(iterator);
				continue;
			}

			++iterator;
		}

		if (!addRequestedwidgetList.empty())
		{
			for (const std::shared_ptr<Widget>& widget : addRequestedwidgetList)
			{
				widgetList.emplace_back(widget);
			}
			addRequestedwidgetList.clear();
		}
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

	void Level::OnSpawnedActor(std::shared_ptr<Actor> spawnedActor)
	{
		assert(spawnedActor && "Invalid spawnedActor");

		/* 생성된 Actor의 위치 업데이트 이벤트에 대한 콜백을 등록한다. */
		spawnedActor->SetUpdatedPositionCallback(std::bind(&Level::OnPositionUpdateActorInLevel, 
															this, 
															std::placeholders::_1, 
															std::placeholders::_2, 
															std::placeholders::_3));

		/* 생성된 Actor의 Destory 이벤트에 대한 콜백을 등록한다. */
		spawnedActor->SetOnDestroyedCallback(std::bind(&Level::OnDestroyedActorInLevel, this, std::placeholders::_1));
	}

	void Level::OnPositionUpdateActorInLevel(std::weak_ptr<Actor> updatedActor, 
											const Vector2Int& prevWorldPosition, 
											const Vector2Int& worldPosition)
	{
	
	}

	void Level::OnDestroyedActorInLevel(std::weak_ptr<Actor> destoryedActor)
	{
	
	}
}
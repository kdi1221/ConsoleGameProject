#include "LoadingLevel.h"
#include <Render/Renderer.h>
#include <Engine/Engine.h>
#include <Resource/ResourceManager.h>
#include "Level/MainMenuLevel.h"

using namespace Craft;

const std::wstring LoadingLevel::loadingAsciiArts[] =
{
	L"█      ███   ███  ████  ███ █   █  ███             ",
	L"█     █   █ █   █ █   █  █  ██  █ █                ",
	L"█     █   █ █████ █   █  █  █ █ █ █  ██            ",
	L"█     █   █ █   █ █   █  █  █  ██ █   █   █   █   █",
	L"█████  ███  █   █ ████  ███ █   █  ███    █   █   █"
};

LoadingLevel::LoadingLevel()
{
	timerSubStrOffsetUpdate.SetTargetTime(0.1f);
}

LoadingLevel::~LoadingLevel()
{
	if (worker.joinable())
	{
		/* 로딩중간에 게임 종료시 스레드 종료까지 대기 */
		worker.join();
	}
}

void LoadingLevel::OnInitialized()
{
	super::OnInitialized();

	/* 로딩 스레드 워커 시작 */
	worker = std::thread([this]()
		{
			LoadResource();
			LoadSound();
			loadComplete = true;
		}
	);
}

void LoadingLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (loadComplete)
	{
		if (worker.joinable())
		{
			/* 완료된 작업 스레드 정리 */
			worker.join();
		}

		/* 메인 메뉴로 전환 */
		Engine::Get().AddNewLevel<MainMenuLevel>();
	}
	else
	{
		/* 로딩 메시지 오프셋 */
		timerSubStrOffsetUpdate.Tick(deltaTime);
		if (timerSubStrOffsetUpdate.IsTimeOut())
		{
			--loadingSubStrOffset;
			if (loadingSubStrOffset < 0)
			{
				loadingSubStrOffset = 12;
			}

			timerSubStrOffsetUpdate.Reset();
		}
	}
}

void LoadingLevel::Draw()
{
	super::Draw();

	Vector2Int titleDrawPos(125, 48);
	for (const std::wstring& titleAscii : loadingAsciiArts)
	{
		Renderer::Get().SubmitUI(titleAscii.substr(0, titleAscii.length() - loadingSubStrOffset), titleDrawPos, Color::BrightYellow);
		++titleDrawPos.y;
	}
}

void LoadingLevel::LoadResource()
{
	ResourceManager& resourceManager = Engine::Get().GetResourceManager<ResourceManager>();
	resourceManager.LoadResources();
}

void LoadingLevel::LoadSound()
{
	Engine& engine = Engine::Get();

	engine.LoadSound("BGM/Mainmenu.wav");
	engine.LoadSound("BGM/stage1.wav");
	engine.LoadSound("BGM/stage2.wav");
	engine.LoadSound("BGM/stage3.wav");
	engine.LoadSound("BGM/BossIntro.wav");
	engine.LoadSound("BGM/BossMain.wav");
	engine.LoadSound("BGM/GameOver.wav");
	engine.LoadSound("BGM/Ending.wav");
}


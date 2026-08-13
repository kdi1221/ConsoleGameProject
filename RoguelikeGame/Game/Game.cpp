#include "Game.h"
#include "Config/Config.h"
#include "Navigation/NavigationTilemap.h"
#include "Game/State/GameMode/GM_Roguelike.h"
#include "Game/State/PlayerState/PS_Roguelike.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"
#include "Types/Enums.h"
#include <Level/TilemapLevel.h>
#include <cassert>

using namespace Craft;

Game::Game()
{

}

void Game::Tick(float deltaTime)
{
	FormatCurrentFPSString(deltaTime);

	Engine::Tick(deltaTime);
}

void Game::Draw()
{
	DrawFPSString();

	Engine::Draw();
}

void Game::ProcessColiision()
{
	//기존 AABB기반의 충돌처리가 아닌 타일 기반의 충돌처리
	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(mainLevel);
	if (!tilemapLevel)
	{
		return;
	}

	tilemapLevel->ProcessTilemapCollision();
}

std::unique_ptr<Craft::ConfigBase> Game::CreateConfig() const
{
	return std::make_unique<Config>();
}

std::unique_ptr<Craft::NavigationBase> Game::CreateNavigationSystem() const
{
	return std::make_unique<NavigationTilemap>();
}

std::unique_ptr<GameMode> Game::CreateGameMode() const
{
	return std::make_unique<GM_Roguelike>();
}

std::unique_ptr<PlayerState> Game::CreatePlayerState() const
{
	return std::make_unique<PS_Roguelike>();
}

void Game::FormatCurrentFPSString(const float deltaTime)
{
	wchar_t szFormat[256] = { 0 };
	const float fps = 1.f / deltaTime;
	swprintf_s(szFormat, L"DeltaTime: %f, FPS : %f", deltaTime, fps);
	szFPS = szFormat;
}

void Game::DrawFPSString()
{
	const Craft::ConfigBase& configBase = Engine::Get().GetConfig<Craft::ConfigBase>();
	Craft::Vector2Int DrawPosition;
	DrawPosition.x = configBase.GetViewWidth() + 1;
	DrawPosition.y = 0;
	Craft::Renderer::Get().SubmitUI(szFPS, DrawPosition, Craft::Color::White, static_cast<int>(eRenderSortingOrder::UI));
}

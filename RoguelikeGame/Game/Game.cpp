#include "Game.h"
#include "Config/Config.h"
#include "Resource/GameResources.h"
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

std::unique_ptr<Craft::ResourceManager> Game::CreateResourceManager() const
{
	return std::make_unique<GameResources>();
}

std::unique_ptr<Craft::NavigationBase> Game::CreateNavigationSystem() const
{
	return std::make_unique<NavigationTilemap>();
}

std::unique_ptr<GameMode> Game::CreateGameMode() const
{
	return std::make_unique<GM_Roguelike>();
}
#include "TestLevel.h"
#include "Engine/Engine.h"
#include "Game/Config/Config.h"
#include "Render/Renderer.h"
#include "Tilemap/Tilemap.h"
#include "Core/Input.h"
#include <cassert>

using namespace Craft;

TestLevel::TestLevel()
{

}

TestLevel::~TestLevel()
{

}

void TestLevel::OnInitialized()
{
	Level::OnInitialized();

	BuildTilemapBSP();
}

void TestLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	if (Input::Get().GetKeyDown('B'))
	{
		BuildTilemapBSP();
	}

	if (tileMap)
	{
		tileMap->Tick(deltaTime);
	}
}

void TestLevel::Draw()
{
	if (tileMap)
	{
		tileMap->Draw();
	}

	Level::Draw();
}

void TestLevel::BuildTilemapBSP()
{
	//이전 타일맵 정보 초기화
	tileMap.reset();

	//새로운 타일맵 생성
	tileMap = std::make_unique<Tilemap>();
	assert(tileMap && "Fail Tilemap Alloc..");

	const Config& config = Engine::Get().GetConfig<Config>();
	tileMap->InitializeTilemap(Vector2Int(config.GetTilemapWidth(), config.GetTilemapHeight()));
}

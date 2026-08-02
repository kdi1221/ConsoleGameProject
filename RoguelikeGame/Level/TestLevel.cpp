#include "TestLevel.h"
#include "Render/Renderer.h"
#include "Tilemap/Tilemap.h"
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

	tileMap = std::make_unique<Tilemap>();
	assert(tileMap && "Fail Tilemap Alloc..");

	tileMap->InitializeTilemap(Vector2(150, 70));
}

void TestLevel::Draw()
{
	if (tileMap)
	{
		tileMap->Draw();
	}

	Level::Draw();
}

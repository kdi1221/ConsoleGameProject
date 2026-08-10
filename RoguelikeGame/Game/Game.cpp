#include "Game.h"
#include "Config/Config.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"
#include "Types/Enums.h"

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

std::unique_ptr<Craft::ConfigBase> Game::CreateConfig() const
{
	return std::make_unique<Config>();
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
	Craft::Vector2Float DrawPosition;
	DrawPosition.x = static_cast<float>(configBase.GetViewWidth() + 1);
	DrawPosition.y = 0.f;
	Craft::Renderer::Get().SubmitUI(szFPS, DrawPosition, Craft::Color::White, static_cast<int>(eRenderSortingOrder::UI));
}

#include "DummyLevel.h"
#include "UI/TextBlock/TextBlockFPS.h"
#include <Engine/Engine.h>
#include <Engine/Config/ConfigBase.h>

using namespace Craft;

DummyLevel::DummyLevel()
{
	
}

DummyLevel::~DummyLevel()
{

}

void DummyLevel::OnInitialized()
{
	super::OnInitialized();

	const ConfigBase& configBase = Engine::Get().GetConfig<Craft::ConfigBase>();
	const int ViewWidth = configBase.GetViewWidth();

	const int widthWidgetFPS = 12;
	const Vector2Int positionWidgetFPS(configBase.GetDisplayWidth() - widthWidgetFPS - 1, 0);
	textBlockFPS = CreateWidget<TextBlockFPS>(positionWidgetFPS, widthWidgetFPS);
}

#include "TestLevel.h"
#include "Actor/TestActor.h"

using namespace Craft;

void TestLevel::OnInitialized()
{
	Level::OnInitialized();

	SpawnActor<TestActor>();
}

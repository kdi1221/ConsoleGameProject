#include "TestLevel.h"
#include "Actor/TestActor.h"

void TestLevel::OnInitialized()
{
	Level::OnInitialized();

	SpawnActor<TestActor>();
}

#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Box.h"
#include "Actor/Ground.h"
#include "Actor/Wall.h"
#include "Actor/Target.h"
#include "Render/Renderer.h"
#include "Math/Vector2Int.h"
#include <fstream>
#include <cassert>

using namespace Craft;

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	//레벨이 시작되면 맵 로드.
	LoadMap("stage1.txt");
}

void GameLevel::Draw()
{
	Level::Draw();

	// 게임을 클리어한 경우 메시지 표시
	if (isGameClear)
	{
		Renderer::Get().Submit("Game Clear!", Vector2Float(30, 0));
	}
}

void GameLevel::LoadMap(const std::string& filename)
{
	// 최종 경로.
	std::string path = std::string("../Assets/Stages/") + filename;

	// 파일 열기 및 확인
	std::ifstream file(path, std::ios_base::binary);
	assert(file.is_open());

	// 파일 전체 크기 확인
	file.seekg(0, std::ios_base::end);
	const std::streampos fileSize = file.tellg();

	// 파일 위치를 첫 위치로 되돌리기
	file.seekg(0, std::ios_base::beg);

	// 파일 내용 전체를 읽기 위한 변수.
	std::string buffer;
	buffer.resize(static_cast<size_t>(fileSize));

	//파일 내용 전체 읽기
	file.read(&buffer[0], fileSize);

	//문자열 buffer를 한 문자씩 읽으면서 처리.
	int index = 0;
	Vector2Int position;

	while (true)
	{
		// 종료 조건.
		if (index >= fileSize)
		{
			break;
		}

		// 현재 순번의 문자 값 읽기.
		char mapCharacter = buffer[index];

		// 다음 글자를 읽기 위한 준비.
		++index;

		// 개행 문자 '\r' 건너뛰기.
		if(mapCharacter == '\r')
		{
			continue;
		}

		// 개행 문자 처리 - 좌표 값 업데이트.
		if(mapCharacter == '\n')
		{
			++position.y;
			position.x = 0;
			continue;
		}

		//읽은 문자 별로 처리.
		switch (mapCharacter)
		{
			// 벽 문자.
		case '#':
			SpawnActor<Wall>(static_cast<Vector2Float>(position));
			break;

			// 땅
		case '.':
			SpawnActor<Ground>(static_cast<Vector2Float>(position));
			break;

			// 플레이어
		case 'p':
			SpawnActor<Ground>(static_cast<Vector2Float>(position));
			SpawnActor<Player>(static_cast<Vector2Float>(position));
			break;

			// 박스
		case 'b':
			SpawnActor<Ground>(static_cast<Vector2Float>(position));
			SpawnActor<Box>(static_cast<Vector2Float>(position));
			break;

			//목표 지점
		case 't':
			SpawnActor<Target>(static_cast<Vector2Float>(position));

			// 타겟의 수를 하나 늘려줌 -> 목표 점수.
			++targetScore;
			break;
		}

		//문자 처리 후 다음 x 위치 업데이트.
		++position.x;
	}

	//파일 닫기
	file.close();
}

bool GameLevel::CheckGameClear()
{
	// 현재 점수 확인용 변수.
	int currentScore = 0;
	
	// 박스 및 목표 지점 액터를 저장하기 위한 배열.
	std::vector<std::shared_ptr<Actor>> boxList;
	std::vector<std::shared_ptr<Actor>> targetList;

	//박스 및 목표 지점 액터 검색
	for(const std::shared_ptr<Actor>& actor : actorList)
	{
		if(actor->IsTypeOf<Box>())
		{
			boxList.emplace_back(actor);
			continue;
		}

		if(actor->IsTypeOf<Target>())
		{
			targetList.emplace_back(actor);
			continue;
		}
	}

	// 목표 지점에 배치된 박스의 수 확인.
	for (const std::shared_ptr<Actor>& box : boxList)
	{
		for (const std::shared_ptr<Actor>& target : targetList)
		{
			//위치 비교
			if (box->GetPosition() == target->GetPosition())
			{
				//박스가 목표 지점에 배치된 경우 점수 +1
				currentScore += 1;
			}
		}
	}

	// 현재 점수가 목표 점수에 도달했는지 여부 확인
	return currentScore == targetScore;
}

bool GameLevel::CanMove(const Craft::Vector2Int& playerPosition, const Craft::Vector2Int& nextPosition)
{
	//예외처리, 게임을 클리어한 경우에는 처리 안함.
	if (isGameClear)
	{
		return false;
	}

	//이 레벨에 배치된 박스 액터를 확인하기 위한 배열.
	std::vector<std::shared_ptr<Actor>> boxList;

	// 레벨을 순회하면서 Box 타입의 액터를 boxList 추가
	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		//커스텀 RTTI를 활용해 Box 액터인 경우 추가
		if (actor->IsTypeOf<Box>())
		{
			boxList.emplace_back(actor);
			continue;
		}
	}

	// 이동하려는 위치에 박스 액터가 있는지 확인하기 위한 변수.
	std::shared_ptr<Actor> boxActor = nullptr;

	// 레벨에 배치된 박스 액터를 순회하면서 이동하려는 위치에 박스가 있는지 확인.
	for (const std::shared_ptr<Actor>& box : boxList)
	{
		// 플레이어가 이동하려는 위치와 같은 위치의 박스가 있는지 확인.
		if (static_cast<Vector2Int>(box->GetPosition()) == nextPosition)
		{
			boxActor = box;
			break;
		}
	}

	// 1. 이동하려는 위치에 박스 액터가 있는 경우.
	// 박스의 다음 위치가 이동 가능한지 확인. 
	if (boxActor)
	{
		// 박스의 다음 위치 구하기.
		// 이를 위해 플레이어가 이동하는 방향 구하기.
		// 이동 방향 : 다음위치 - 현재 위치
		Vector2Int direction = nextPosition - playerPosition;
		//박스의 다음 위치 : 박스의 위치 + 이동 방향. 
		Vector2Int newPosition = static_cast<Vector2Int>(boxActor->GetPosition()) + direction;

		// 다른 박스가 있는지 확인.
		for (const std::shared_ptr<Actor>& otherBox : boxList)
		{
			//비교하려는 박스가 현재 박스인 경우.
			if (otherBox == boxActor)
			{
				continue;
			}

			//이동하려는 위치에 다른 박스가 있는 경우.
			if (static_cast<Vector2Int>(otherBox->GetPosition()) == newPosition)
			{
				return false;
			}
		}

		//이동하려는 곳에 박스가 없다면, 이동 가능한지 추가로 확인.
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 박스가 이동하려는 위치에 있는 액터 검색.
			if (static_cast<Vector2Int>(actor->GetPosition()) == newPosition)
			{
				//이 위치에 벽이 있으면 이동 불가.
				if (actor->IsTypeOf<Wall>())
				{
					return false;
				}

				// 이 위치에 있는 액터가 땅이거나 목표 위치라면 이동 가능.
				if (actor->IsTypeOf<Ground>() || actor->IsTypeOf<Target>())
				{
					// 박스 이동 처리(플레이어가 박스를 미는 효과).
					boxActor->SetPosition(static_cast<Vector2Float>(newPosition));

					// 점수 확인 함수를 구현한 후 게임 클리어 확인
					isGameClear = CheckGameClear();

					return true;
				}
			}
		}
	}

	//2. 이동하려는 위치에 박스가 없는 경우.
	// 플레이어가 벽을 뚫고 가지 못하기 때문에 벽만 확인
	for (const std::shared_ptr<Actor>& actor : actorList)
	{
		//플레이어가 이동하려는 위치의 액터 검색
		if (static_cast<Vector2Int>(actor->GetPosition()) == nextPosition)
		{
			//이 위치에 있는 액터가 벽이라면 이동 불가
			if(actor->IsTypeOf<Wall>())
			{
				return false;
			}

			return true;
		}
	}

	// 이 위치는 도달하면 안됨.
	return false;
}
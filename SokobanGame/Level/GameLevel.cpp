#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Box.h"
#include "Actor/Ground.h"
#include "Actor/Wall.h"
#include "Actor/Target.h"
#include "Math/Vector2.h"
#include <fstream>
#include <cassert>

using namespace Craft;

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	//레벨이 시작되면 맵 로드.
	LoadMap("Map.txt");
}

void GameLevel::Draw()
{
	Level::Draw();
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
	Vector2 position;

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
			SpawnActor<Wall>(position);
			break;

			// 땅
		case '.':
			SpawnActor<Ground>(position);
			break;

			// 플레이어
		case 'p':
			SpawnActor<Ground>(position);
			SpawnActor<Player>(position);
			break;

			// 박스
		case 'b':
			SpawnActor<Ground>(position);
			SpawnActor<Box>(position);
			break;

			//목표 지점
		case 't':
			SpawnActor<Target>(position);

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

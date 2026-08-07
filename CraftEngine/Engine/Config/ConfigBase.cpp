#include "ConfigBase.h"
#include <fstream>
#include <sstream>
#include <cassert>

namespace Craft
{
	ConfigBase::ConfigBase()
	{
		AddLoadConfigCallback("framerate", [this](const std::string& inKey, const std::string& inValue)
			{
				framerate = static_cast<float>(atof(inValue.c_str()));
				assert(framerate > 0.f);
				return framerate > 0.f;
			});

		AddLoadConfigCallback("width", [this](const std::string& inKey, const std::string& inValue)
			{
				width = static_cast<int>(atoi(inValue.c_str()));
				assert(width > 0);
				return width > 0;
			});

		AddLoadConfigCallback("height", [this](const std::string& inKey, const std::string& inValue)
			{
				height = static_cast<int>(atoi(inValue.c_str()));
				assert(height > 0);
				return height > 0;
			});
	}

	ConfigBase::~ConfigBase()
	{

	}

	bool ConfigBase::LoadEngineConfig(const std::string& configPath)
	{
		// 엔진 설정 파일 열기.
		std::ifstream file(configPath);

		// 정상적으로 열렸는지 확인.
		if (!file.is_open())
		{
			return false;
		}

		// 라인(Line) 별로 읽기.
		bool loadResult = true;
		std::string line;
		while (std::getline(file, line))
		{
			// 빈 줄 및 주석 건너뛰기.
			if (line.empty() || line[0] == '#')
			{
				continue;
			}

			// key = value 포맷 파싱.
			const size_t equalPosition = line.find('=');

			// 라인 문자열에 =문자가 있는지 확인.
			assert(equalPosition != std::string::npos);

			// 좌/우 공백 제거용 람다.
			auto trim = [](std::string& s)
				{
					// 공백 문자 집합. (\r\n -> CRLF) (\n -> LF)
					// ' ': 스페이스.
					// \t: 탭
					// \r: 윈도우 개행문자 일자
					// \n: 개행 문자.
					const char* whiteSpace = " \t\r\n";

					// 문자열의 앞에서부터 공백이 아닌 첫 문자 위치 검색.
					const size_t begin = s.find_first_not_of(whiteSpace);

					// 공백이 아닌 문자를 못찾은 경우에는 빈 문자열로 설정 후 반환
					if (begin == std::string::npos)
					{
						s.clear();
						return;
					}

					// 문자열의 뒤에서부터 공백이 아닌 마지막 문자 위치 검색.
					const size_t end = s.find_last_not_of(whiteSpace);

					// 공백 제외한 begin-end 사이의 문자열 반환
					s = s.substr(begin, end - begin + 1);
				};

			// key 파싱
			std::string key = line.substr(0, equalPosition);

			// value 파싱
			std::string value = line.substr(equalPosition + 1);

			// key/value에서 공백 제거.
			trim(key);
			trim(value);

			//key,value의 유효성 확인
			assert(!key.empty() && !value.empty());

			auto findKeyCallback = mapKeyCallback.find(key);
			if (findKeyCallback == mapKeyCallback.end())
			{
				continue;
			}

			//파싱한 키별 콜백함수 호출
			if (false == findKeyCallback->second(key, value))
			{
				loadResult = false;
				break;
			}
		}

		//처리가 완료되면 파일 닫기.
		file.close();

		return loadResult;
	}

	void ConfigBase::AddLoadConfigCallback(const std::string& inKey, const CallBack_Type& inCallbackInfo)
	{
		auto findCallback = mapKeyCallback.find(inKey);
		if (findCallback != mapKeyCallback.end())
		{
			return;
		}

		mapKeyCallback.insert(std::pair<std::string, CallBack_Type>(inKey, inCallbackInfo));
	}
}

#pragma once

#include "Core/Core.h"
#include <string>
#include <map>
#include <functional>

namespace Craft
{
	class CRAFT_API ConfigBase
	{
	protected:
		using CallBack_Type = std::function<bool(const std::string&, const std::string&)>;

	public:
		ConfigBase();
		virtual ~ConfigBase();

	public:
		bool LoadEngineConfig(const std::string& configPath);

	protected:
		void AddLoadConfigCallback(const std::string& inKey, const CallBack_Type& inCallbackInfo);

	public:
		inline float GetFrameRate() const { return framerate; }
		inline int GetDisplayWidth() const { return width; }
		inline int GetDisplayHeight() const { return height; }
		inline int GetViewWidth() const { return viewWidth; }
		inline int GetViewHeight() const { return viewHeight; }

	private:
		// 목표 프레임 속도.
		float framerate = 0.f;

		// 화면 가로 크기
		int width = 0;

		// 화면 세로 크기
		int height = 0;

		// 카메라 뷰 가로 크기
		int viewWidth = 0;

		// 카메라 뷰 세로 크기
		int viewHeight = 0;

	private:
		//파싱된 Config Key마다 호출될 콜백
		std::map<std::string, CallBack_Type> mapKeyCallback;

	};
}




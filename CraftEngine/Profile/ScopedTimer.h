#pragma once

#include "Core/Core.h"
#include <cwchar>
#include <Windows.h>

/* 프로파일링, 범위 타이머 */
class CRAFT_API ScopedTimer
{
public:
    explicit ScopedTimer(const wchar_t* name)
        : name(name)
    {
        QueryPerformanceCounter(&startTime);
    }

    ~ScopedTimer()
    {
        LARGE_INTEGER endTime;
        QueryPerformanceCounter(&endTime);

        const double elapsedMs =
            static_cast<double>(endTime.QuadPart - startTime.QuadPart)
            * 1000.0
            / static_cast<double>(GetFrequency().QuadPart);

        wchar_t buffer[256] = { 0 };
        swprintf_s(
            buffer,
            _countof(buffer),
            L"[%s] %.3f ms\n",
            name,
            elapsedMs);

        OutputDebugStringW(buffer);
    }

private:
    static const LARGE_INTEGER& GetFrequency()
    {
        static const LARGE_INTEGER frequency = []()
            {
                LARGE_INTEGER value;
                QueryPerformanceFrequency(&value);
                return value;
            }();

        return frequency;
    }

private:
    const wchar_t* name = nullptr;
    LARGE_INTEGER startTime{};
};

#define PROFILE_SCOPE(Name) \
    ScopedTimer scopedTimer_##__LINE__(Name)
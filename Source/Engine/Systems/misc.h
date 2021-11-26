#pragma once

#include <windows.h>
#include <crtdbg.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _ASSERT_EXPR_A(expr, msg) \
	(void)((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || \
	(_CrtDbgBreak(), 0))
#else
#define  _ASSERT_EXPR_A(expr, expr_str) ((void)0)
#endif

inline LPWSTR hr_Trace(HRESULT hr)
{
	LPWSTR msg;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msg), 0, NULL);
	return msg;
}

/// <summary>
///
/// </summary>
class Benchmark
{
	LARGE_INTEGER ticks_per_second;
	LARGE_INTEGER start_ticks;
	LARGE_INTEGER current_ticks;

public:
	Benchmark()
	{
		QueryPerformanceFrequency(&ticks_per_second);
		QueryPerformanceCounter(&start_ticks);
		QueryPerformanceCounter(&current_ticks);
	}
	void Begin()
	{
		QueryPerformanceCounter(&start_ticks);
	}
	float End()
	{
		QueryPerformanceCounter(&current_ticks);
		return static_cast<float>(current_ticks.QuadPart - start_ticks.QuadPart) / static_cast<float>(ticks_per_second.QuadPart);
	}
};
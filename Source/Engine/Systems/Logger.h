#pragma once

// ToDo Loggerが行うことを書く

class Logger
{
private:

public:
	// ログ出力
	static void Print(const char* format, ...);
};
//https://cpprefjp.github.io/lang/cpp11/variadic_macros.html
#if 	defined(_DEBUG)
#define	LOG(...)		{ Logger::Print(__VA_ARGS__); }
#else
#define	LOG(...)		{}
#endif

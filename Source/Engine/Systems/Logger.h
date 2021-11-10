#pragma once

// ToDo Logger‚ªs‚¤‚±‚Æ‚ğ‘‚­

class Logger
{
private:

public:
	// ƒƒOo—Í
	static void Print(const char* format, ...);
};
//https://cpprefjp.github.io/lang/cpp11/variadic_macros.html
#if 	defined(_DEBUG)
#define	LOG(...)		{ Logger::Print(__VA_ARGS__); }
#else
#define	LOG(...)		{}
#endif

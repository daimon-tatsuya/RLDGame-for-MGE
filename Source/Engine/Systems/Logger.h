#pragma once

// ToDo Logger���s�����Ƃ�����

class Logger
{
private:

public:
	// ���O�o��
	static void Print(const char* format, ...);
};
//https://cpprefjp.github.io/lang/cpp11/variadic_macros.html
#if 	defined(_DEBUG)
#define	LOG(...)		{ Logger::Print(__VA_ARGS__); }
#else
#define	LOG(...)		{}
#endif

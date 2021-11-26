#pragma once
//**********************************************************
//
//		Logger�N���X
//
//**********************************************************

/// <summary>
/// �@Visual Stadio�̏o�̓E�B���h�E�Ƀf�o�b�O��������o�͂���N���X
/// </summary>
class Logger
{
private:

public:
	/// <summary>
	/// �@���O�o��
	/// </summary>
	/// <param name="format">�o�͓��e</param>
	/// <param name="">�ϒ�����</param>
	static void Print(const char* format, ...);
};
//__VA_ARGS__�@https://cpprefjp.github.io/lang/cpp11/variadic_macros.html
#if 	defined(_DEBUG)
#define	LOG(...)		{ Logger::Print(__VA_ARGS__); }
#else
#define	LOG(...)		{}
#endif

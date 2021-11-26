#pragma once
//**********************************************************
//
//		Loggerクラス
//
//**********************************************************

/// <summary>
/// 　Visual Stadioの出力ウィンドウにデバッグ文字列を出力するクラス
/// </summary>
class Logger
{
private:

public:
	/// <summary>
	/// 　ログ出力
	/// </summary>
	/// <param name="format">出力内容</param>
	/// <param name="">可変長引数</param>
	static void Print(const char* format, ...);
};
//__VA_ARGS__　https://cpprefjp.github.io/lang/cpp11/variadic_macros.html
#if 	defined(_DEBUG)
#define	LOG(...)		{ Logger::Print(__VA_ARGS__); }
#else
#define	LOG(...)		{}
#endif

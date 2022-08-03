//**********************************************************
//
//		Loggerクラス
//
//**********************************************************

#include <Windows.h>
#include <cstdio>

#include "Liblary/Systems/Logger.h"

// ログ出力
void Logger::Print(const char* format, ...)
{
	char message[1024];

	va_list args;
	//可変個数引数のリストを初期化
	va_start(args, format);

	//引数リストへのポインターを使用して、書式付き出力を書き込み
	vsnprintf(message, sizeof(message), format, args);

	//可変個数引数のリストをクリア
	va_end(args);

	//出力ウィンドウにデバッグ文字列を出力する
	::OutputDebugStringA(message);
}
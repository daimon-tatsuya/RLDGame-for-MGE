#pragma once

#include <windows.h>

//ToDo HighResolutionTimerコメント

class HighResolutionTimer
{
private:
	double seconds_per_count;
	double delta_time;

	LONGLONG base_time;
	LONGLONG paused_time;
	LONGLONG stop_time;
	LONGLONG last_time;
	LONGLONG this_time;

	bool stopped;

public:


private:


public:
	HighResolutionTimer();

	// メッセージループの前に呼び出す
	void Reset();

	// 一時停止していない時の呼び出し
	void Start();

	// 一時停止中に呼び出す
	void Stop();

	// すべてのフレームを呼び出す。
	void Tick();

	// 単位：秒
	float TimeInterval() const;

	// Reset()が呼ばれてからの経過時間を、時計が止まっている時間を除いて返します。
	// 単位：秒
	float TimeStamp() const;




};

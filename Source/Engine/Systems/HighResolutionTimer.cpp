#include "Engine/Systems/HighResolutionTimer.h"

HighResolutionTimer::HighResolutionTimer() : delta_time(-1.0), paused_time(0), stopped(false), stop_time(0)
{
	LONGLONG counts_per_sec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&counts_per_sec));
	seconds_per_count = 1.0 / static_cast<double>(counts_per_sec);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this_time));
	base_time = this_time;
	last_time = this_time;
}

void HighResolutionTimer::Reset()
{
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this_time));
		base_time = this_time;
		last_time = this_time;

		stop_time = 0;
		stopped = false;
	}
}

void HighResolutionTimer::Start()
{
	LONGLONG start_time;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time));

	// ストップとスタートのペアの間の経過時間を蓄積する。
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  base_time       stop_time        start_time
	if (stopped)
	{
		paused_time += (start_time - stop_time);
		last_time = start_time;
		stop_time = 0;
		stopped = false;
	}
}

void HighResolutionTimer::Stop()
{
	if (!stopped)
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&stop_time));
		stopped = true;
	}
}

void HighResolutionTimer::Tick()
{
	if (stopped)
	{
		delta_time = 0.0;
		return;
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this_time));

	// このフレームと前のフレームとの時間差。

	delta_time = (this_time - last_time) * seconds_per_count;

	// 次のフレームの準備
	last_time = this_time;

	// 強制的に非負の値にする。
	//DXSDKのCDXUTTimerでは、プロセッサがパワーセーブモードになったり、
	//他のプロセッサにシャッフルされたりした場合、
	//delta_timeが負の値になることがあると言及しています。
	if (delta_time < 0.0)
	{
		delta_time = 0.0;
	}
}


float HighResolutionTimer::TimeInterval() const
{
	return static_cast<float>(delta_time);
}

float HighResolutionTimer::TimeStamp() const
{
	// 停止している場合は、停止してからの経過時間をカウントしない。
	// さらに、すでに一時停止していた場合、stop_time - base_timeには、カウントしたくない一時停止の時間が含まれます。
	// これを修正するには、stop_timeから一時停止した時間を差し引きます。

	//                     |<--paused_time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  base_time       stop_time        start_time     stop_time    this_time

	if (stopped)
	{
		return static_cast<float>(((stop_time - paused_time) - base_time) * seconds_per_count);
	}

	// this_time - base_timeの距離には、一時停止時間が含まれています。
	// これを修正するには、this_timeから一時停止した時間を引きます。
	//  (this_time - paused_time) - base_time
	//
	//                     |<--paused_time-->|
	// ----*---------------*-----------------*------------*------> time
	//  base_time       stop_time        start_time     this_time
	else
	{
		return static_cast<float>(((this_time - paused_time) - base_time) * seconds_per_count);
	}
}



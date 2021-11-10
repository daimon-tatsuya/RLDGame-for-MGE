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

	// �X�g�b�v�ƃX�^�[�g�̃y�A�̊Ԃ̌o�ߎ��Ԃ�~�ς���B
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

	// ���̃t���[���ƑO�̃t���[���Ƃ̎��ԍ��B

	delta_time = (this_time - last_time) * seconds_per_count;

	// ���̃t���[���̏���
	last_time = this_time;

	// �����I�ɔ񕉂̒l�ɂ���B
	//DXSDK��CDXUTTimer�ł́A�v���Z�b�T���p���[�Z�[�u���[�h�ɂȂ�����A
	//���̃v���Z�b�T�ɃV���b�t�����ꂽ�肵���ꍇ�A
	//delta_time�����̒l�ɂȂ邱�Ƃ�����ƌ��y���Ă��܂��B
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
	// ��~���Ă���ꍇ�́A��~���Ă���̌o�ߎ��Ԃ��J�E���g���Ȃ��B
	// ����ɁA���łɈꎞ��~���Ă����ꍇ�Astop_time - base_time�ɂ́A�J�E���g�������Ȃ��ꎞ��~�̎��Ԃ��܂܂�܂��B
	// ������C������ɂ́Astop_time����ꎞ��~�������Ԃ����������܂��B

	//                     |<--paused_time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  base_time       stop_time        start_time     stop_time    this_time

	if (stopped)
	{
		return static_cast<float>(((stop_time - paused_time) - base_time) * seconds_per_count);
	}

	// this_time - base_time�̋����ɂ́A�ꎞ��~���Ԃ��܂܂�Ă��܂��B
	// ������C������ɂ́Athis_time����ꎞ��~�������Ԃ������܂��B
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



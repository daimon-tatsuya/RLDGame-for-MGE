#pragma once

//**********************************************************
//
//	!	HighResolutionTimer�N���X
//
//**********************************************************

#include <Windows.h>

/// <summary>
/// /�t���[���Ǘ��N���X
/// </summary>
class HighResolutionTimer final
{
private:
	double seconds_per_count{};
	double delta_time{};

	LONGLONG base_time{};
	LONGLONG paused_time{};
	LONGLONG stop_time{};
	LONGLONG last_time{};
	LONGLONG this_time = static_cast< LONGLONG>(0.f);

	bool stopped = false;

public:

private:

public:
	HighResolutionTimer();

	// ���b�Z�[�W���[�v�̑O�ɌĂяo��
	void Reset();

	// �ꎞ��~���Ă��Ȃ����̌Ăяo��
	void Start();

	// �ꎞ��~���ɌĂяo��
	void Stop();

	// ���ׂẴt���[�����Ăяo���B
	void Tick();

	// �P�ʁF�b
	float TimeInterval() const;

	// Reset()���Ă΂�Ă���̌o�ߎ��Ԃ��A���v���~�܂��Ă��鎞�Ԃ������ĕԂ��܂��B
	// �P�ʁF�b
	float TimeStamp() const;
};

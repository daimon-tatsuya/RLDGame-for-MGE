//**********************************************************
//
//		Logger�N���X
//
//**********************************************************

#include <Windows.h>
#include <cstdio>

#include "Liblary/Systems/Logger.h"

// ���O�o��
void Logger::Print(const char* format, ...)
{
	char message[1024];

	va_list args;
	//�ό������̃��X�g��������
	va_start(args, format);

	//�������X�g�ւ̃|�C���^�[���g�p���āA�����t���o�͂���������
	vsnprintf(message, sizeof(message), format, args);

	//�ό������̃��X�g���N���A
	va_end(args);

	//�o�̓E�B���h�E�Ƀf�o�b�O��������o�͂���
	::OutputDebugStringA(message);
}
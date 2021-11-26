#pragma once
//**********************************************************
//
//		CameraController�N���X
//
//**********************************************************

#include <windows.h>

#include "Engine/Systems/Math.h"


/// <summary>
/// �J�����̋������s���N���X
/// </summary>
class CameraController
{
private:
	// ���[�h
	enum class Mode
	{
		FreeCamera,		// �t���[�J����
		LockonCamera,	// ���b�N�I���J����
		MotionCamera,	// ���[�V�����J����
		DebugCamera,    //�f�o�b�N�J����
	};
	DirectX::XMFLOAT3	position = { 0, 0, 0 };//�ʒu
	DirectX::XMFLOAT3	target = { 0, 0, 0 };//�����_
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };//�e���̉�]�p(���W�A��)
	DirectX::XMFLOAT3	new_position = { 0, 0, 0 };//
	DirectX::XMFLOAT3	new_target = { 0, 0, 0 };

	float				roll_speed = DirectX::XMConvertToRadians(90);//��]���x
	float				range = 10.0f;
	float				max_angleX = DirectX::XMConvertToRadians(+45);//�ő��]�p
	float				min_angleX = DirectX::XMConvertToRadians(-45);//�ŏ���]�p
	float              camera_heght = 50.f;//�J�����̍���
	//ImGui
	bool               ViewMap = false;
public:

private:

public:

	CameraController();
	~CameraController() {};

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void Update(float elapsed_time);

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

};

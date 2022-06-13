#pragma once
//**********************************************************
//
//		SceneGame�N���X
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"


//�O���錾
class Player;
class CameraController;

//class AxisAlignedBoundingBox;�� ����

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	std::unique_ptr<CameraController> camera_controller = nullptr; //�J��������N���X�̃|�C���^

	bool is_darking = true; //��ʂ��Â�����Ƃ�true
	bool is_next_floor = false;//���̃t���A�ɐi�ނȂ�true
private:
	//�K���ړ�����
	void GoingToTheNextFloor();

	//�_���W�������폜����
	void ClearFloor();

	//���̃_���W�����𐶐��A�z�u
	void CreateNextFloor();

public:

	SceneGame() = default;

	~SceneGame() override;

	void Initialize() override;

	void Finalize() override;

	void Update(float elapsed_time) override;

	void Render() override;

	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram) override;

	//���^AI�Ƀ^�[���̏I����`����
	void SendMessaging(MESSAGE_TYPE msg)override;
};

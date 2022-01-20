#pragma once
//**********************************************************
//
//		SceneGame�N���X
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"
#include "Engine/AI/DungeonMake.h"



//�O���錾
class Player;
class CameraController;
class AxisAlignedBoundingBox;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	SceneGame() = default;

	~SceneGame() override;

	void Initialize()override;

	void Finalize() override;

	void Update(float elapsed_time)override;

	void Render()override;

private:

public:
	std::unique_ptr<Player>   player= nullptr ;
	std::unique_ptr<CameraController> camera_controller = nullptr;
	RogueLikeDungeon storage_dungeon{};//�V�[���������Ă���}�b�v���
	//std::vector<AxisAlignedBoundingBox*> axis_aligned_bounding_box_for_frustum;// ������J�����O�p��AABB
	//AxisAlignedBoundingBox* axis_aligned_bounding_box[32]{};

	//�K�v�Ȃ�g���̂ŏ����Ȃ�
	//float	timer = 0.0f;
	//int		counter = 0;
	//char	message[32] = { 0 };
};

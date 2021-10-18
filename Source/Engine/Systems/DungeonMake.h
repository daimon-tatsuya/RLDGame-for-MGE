#pragma once
#include <cstdint>
#include <vector>
#include <DirectXMath.h>
#include "Engine/Systems/Math.h"
//�Q�l����
//https://qiita.com/gis/items/f760a1fb27aa4c3659cf
//https://qiita.com/gis/items/b9cf998db63b465d9f50

#define ONE 1 //DXlib��GetRand�֐����g��Ȃ��ꍇ+1����K�v������

/*�}�b�v�n�f�[�^*/
const static size_t MapSize_Y = 64; //�}�b�v�c�T�C�Y
const static size_t MapSize_X = 32;   //�}�b�v���T�C�Y
const static int MapSize = MapSize_Y * MapSize_X;

const static int Mob_Max = 25;//mob�̏o���ő�l
const static int Cell_Size = 2;//world���W�ł̏��ڂ̑傫��
/// <summary>
/// mob�̃}�b�v���
/// </summary>
struct MobRole
{
	DirectX::XMFLOAT2 position = {};//�}�b�v��ʒu
};

/// <summary>
/// �}�b�v�̐��`
/// </summary>
struct DungeonMapRole
{
	//��������镔���̐� (���m�Ɍ����Ɛ����������̐�)
	size_t division_count_min = 3; //�}�b�v�̋敪���ŏ���
	size_t division_count_rand = 4; //�}�b�v�̋敪�������Z

	//��������镔���̃T�C�Y
	size_t room_length_minX = 5; //������X���W�̍ŏ��T�C�Y
	size_t room_length_minY = 5; //������Y���W�̍ŏ��T�C�Y
	size_t room_length_randX = 2; //������X���W�̃T�C�Y���Z
	size_t room_length_randY = 2; //������Y���W�̃T�C�Y���Z

	size_t map_division_count = {}; //�}�b�v�̋敪���� (�����̌�) 0~n�܂ł̕���ID
	size_t map_division[8][4] = {}; //�}�b�v�̋�� [����ID][Y�I�_ , X�I�_ , Y�n�_ , X�n�_]
	size_t map_room_id[8] = {};   //������ID
	size_t map_room_count = 0;  //�����̐�
	size_t map_room[8][4] = {}; //�}�b�v�̕��� [����ID][Y�I�_ , X�I�_ , Y�n�_ , X�n�_]
	size_t map_road[8][4] = {}; //�}�b�v�̓� [����ID(�O)][�q�����̕���ID(��) , (0.Y���W , 1.X���W) , (�O)���̒ʘH�̈ʒu , (��)���̒ʘH�̈ʒu]
	size_t map_room_player[8] = {};
	size_t map_room_area[8] = { 0 }; //�����̖ʐ�
};

/// <summary>
/// ���������\�ȃ}�b�v
/// </summary>
class RogueLikeMap
{
public:
	RogueLikeMap(const size_t var_) :map_data(var_) {}
	RogueLikeMap() = default;
	//ToDo enum class�����
	size_t map_data = 1;// 0:�ǁA1:���A2:�v���C���[�A3:�G, 4 : �A�C�e��, 5 : �
};

/// <summary>
/// ����̃}�b�v�f�[�^������
/// </summary>
class  RogueLikeDungeon
{
public:
	RogueLikeDungeon();
	~RogueLikeDungeon();


	/// <summary>
	/// �}�b�v�̃T�C�Y������������
	/// </summary>
	void InitializeMapSize();

	/// <summary>
	///  �C���X�^���X�擾
	/// </summary>
	/// <returns>���̃N���X�̎���</returns>
	//static RogueLikeDungeon& Instance()
	//{
	//	static RogueLikeDungeon instance;
	//	return instance;
	//}

	/// <summary>
	/// �}�b�v�����֐�
	/// </summary>
	/// <param name="dungeon_map_role">�}�b�v���</param>
	/// <returns></returns>
	bool MapMake(DungeonMapRole* const dungeon_map_role/*, std::vector<std::vector<RogueLikeMap>>& map_role*/);

	/// <summary>
	/// �}�b�v�������ƂɃ}�b�v���mob��z�u����֐�
	/// </summary>
	/// <param name="dungeon_map_role">�}�b�v���</param>
	/// <param name="id">�}�b�v���mob�ɕt�^����id</param>
	/// <returns></returns>
	bool MobMake(DungeonMapRole* dungeon_map_role, std::vector<std::vector<RogueLikeMap>>& map_role, int id);

	/// <summary>
	/// �V�K�_���W�����쐬�̊֐�
	/// </summary>
	/// <param name="dungeon_map_role">�}�b�v���</param>
	void DungeonMake(DungeonMapRole* dungeon_map_role);

	/// <summary>
	/// �}�b�v��������
	/// </summary>
	void MapClear();

	/// <summary>
	/// �}�b�v����������
	/// </summary>
	void MapReMake(DungeonMapRole* dungeon_map_role);
private:

public:
	/// <summary>
	/// �}�b�v�̒ʘH�̎�
	/// </summary>
	enum class  Road : int
	{
		Axis_Y = 0,
		Axis_X
	};

	/// <summary>
	/// �}�b�v����ۑ�����R���e�i
	/// 0:�ǁA1:���A2:�v���C���[�A3:�G, 4:�A�C�e��, 5:�
	/// </summary>
	std::vector<std::vector<RogueLikeMap>> map_role;
	/// <summary>
	/// �}�b�v���̐��`�@�}�b�v�v�[��
	/// </summary>
	DungeonMapRole dungeon_map_role = {};
	/// <summary>
	/// �}�b�v��ɑ��݂���mob�̏��
	/// </summary>
	MobRole mobs[Mob_Max] = {};
};

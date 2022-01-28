#pragma once
//**********************************************************
//
//		RogueLikeDungeon�N���X
//
//**********************************************************
#include <vector>
#include "Engine/Systems/Math.h"

//�Q�l����
//https://qiita.com/gis/items/f760a1fb27aa4c3659cf
//https://qiita.com/gis/items/b9cf998db63b465d9f50
//http://koyoru1214.blog.fc2.com/blog-entry-7.html
//http://koyoru1214.blog.fc2.com/blog-entry-13.html

// �O���[�o���ϐ�
/*!�}�b�v�n�f�[�^*/

//XY�@�ő�ɂ���48�@����ȏ��fps���ቺ������
static int	MapSize_Y = 48;	// �}�b�v�c�T�C�Y
static int	MapSize_X = 48;   // �}�b�v���T�C�Y
static int	MapSize = MapSize_Y * MapSize_X;


constexpr static int MobMax = 25;	// mob�̏o���ő�l
constexpr static int RoomMax = 8;	// �����̐�

constexpr static int	 ONE = 1; // DXlib��GetRand�֐����g��Ȃ��ꍇ+1����K�v������

constexpr static int CellSize = 2;	// world���W�ł̏��ڂ̑傫��

//constexpr static int PlayerID = 0;	// �v���C���[��ID


//constexpr  static int EndPointY = 0;	// Y�I�_
//constexpr  static int EndPointX = 1;	// X�I�_
//constexpr  static int StartPointY = 2;	// Y�n�_
//constexpr  static int StartPointX = 3;	// X�n�_


enum class Attribute
{
	Wall = 0,// 0:��
	Floor,    // 1:��
	Room,	// 2:����
	Road,	// 3:��
	Player,   // 4:�v���C���[
	Enemy,  // 5:�G
	Exit,		// 6:�K�i
	// 6 : �A�C�e��
	// 7 : �
	// 8 : �A�C�e���̏�̃v���C���[
	// 9 : �A�C�e���̏�̓G
	// 10 : �n�`�̏�̃v���C���[
	// 11 :  �n�`�̏�̓G
	EndAttribute
};


// ���������\�ȃ}�b�v
struct RogueLikeMap
{
	RogueLikeMap(const size_t var) :map_data(var) {}
	RogueLikeMap() = default;

	size_t map_data = static_cast<size_t>(Attribute::Floor);// �}�X�̑������

	//���̓�����������łȂ����̃t���O
	bool road_entrance_flag = false; //  false:�� �Atrue:���̓����

	//�v���[���[��G�A�A�C�e��������Ă��Ă����ݓI�ɕ�����悤����t���O
	bool is_this_room_flag = false; // false:�ʘH�Atrue:�����@���ǂ͕����ł͂Ȃ�����(false�̂܂�)�Ƃ���B
														//\�܂��A�V���ɑ�����ꍇ�͂��̃t���O�������āAenum class �Ŕ��ʂ���悤�ɂ���
};


// ����̃}�b�v�f�[�^������
class  RogueLikeDungeon
{
private:

public:

	//struct RogueLikeMap
	//{
	//	RogueLikeMap(const size_t var) :map_data(var) {}
	//	RogueLikeMap() = default;

	//	size_t map_data = static_cast<size_t>(Attribute::Floor);// �}�X�̑������

	//	//�v���[���[��G�A�A�C�e��������Ă��Ă����ݓI�ɕ�����悤����t���O
	//	bool is_this_room_flag = false; // false:�ʘH�Atrue:�����@0:1�Ɋ��o�@���ǂ͒ʘH����(false�̂܂�)�Ƃ���B�܂��A�V���ɑ�����ꍇ��enum class �Ŕ��ʂ���悤�ɂ���
	//};

	enum class  Road : int//  �}�b�v�̒ʘH�̎�
	{
		Axis_Y = 0,
		Axis_X
	};

	// mob�̃}�b�v���
	struct MobRole
	{
		DirectX::XMFLOAT2 position = {};// �}�b�v��ʒu
		int id{};//�}�b�v���mob��ID
	};


	// �}�b�v�̐��`
	struct DungeonMapRole
	{
		//��������镔���̐� (���m�Ɍ����Ɛ����������̐�)
		size_t division_count_min = 4; // �}�b�v�̋敪���ŏ���
		size_t division_count_rand = 4; // �}�b�v�̋敪�����ɉ��Z����ő�l
		size_t division_count_max = division_count_rand + division_count_min; // �}�b�v�̋敪���ő�

		//��������镔���̃T�C�Y
		size_t room_length_minX = 5;  // ������X���W�̍ŏ��T�C�Y
		size_t room_length_minY = 5;  // ������Y���W�̍ŏ��T�C�Y
		size_t room_length_randX = 5; // ������X���W�̃T�C�Y���Z
		size_t room_length_randY = 5; // ������Y���W�̃T�C�Y���Z

		size_t map_division_count{};					// �}�b�v�̋敪���� (�����̌�) 0~n�܂ł̕���ID
		size_t map_division[RoomMax][4]{};		// �}�b�v�̋�� [����ID][Y�I�_ , X�I�_ , Y�n�_ , X�n�_]
		size_t map_room_id[RoomMax]{};			// ������ID
		size_t map_room_count{};						// �����̐�
		size_t map_room[RoomMax][4]{};			// �}�b�v�̕��� [����ID][Y�I�_ , X�I�_ , Y�n�_ , X�n�_]
		size_t map_road[RoomMax][4]{};			// �}�b�v�̓� [����ID(�O)] [�q�����̕���ID(��) , (0:Y���W , 1:X���W) , (�O)���̒ʘH�̈ʒu , (��)���̒ʘH�̈ʒu]
		size_t map_room_area[RoomMax]{};		// �����̖ʐ�
	};

	std::vector<std::vector<RogueLikeMap>> map_role{};// �}�b�v�����i�[����R���e�i

	std::vector<DirectX::XMINT2>  roads_entrance{};//���̓�����̈ʒu�������i�[����R���e�i

	DungeonMapRole dungeon_map_role{};// �}�b�v���̐��`�@�}�b�v�v�[��

	MobRole mobs[MobMax]{};// �}�b�v��ɑ��݂���mob�̏��

	size_t map_room_player{};	// Player�̂��镔���̔ԍ�


private:

public:

	RogueLikeDungeon();
	~RogueLikeDungeon();

	//�}�b�v�̃T�C�Y������������
	void InitializeMapSize();

	// �G�̍X�V�̑O�Ƀv���C���[�̃}�b�v�����X�V����
	void UpdateMapRolePlayer();

	// �G�̃}�b�v�����X�V����
	void UpdateMapRoleEnemis();

	// �}�b�v�����֐�
	bool MapMake();

	/// <summary>
	/// �}�b�v�������ƂɃ}�b�v���mob��z�u����֐�
	/// </summary>
	/// <param name="id">�}�b�v���mob�ɕt�^����id</param>
	/// <returns></returns>
	bool MobMake(int id);

	//�V�K�_���W�����쐬�̊֐�
	void DungeonMake();


	// �}�b�v��������
	void MapClear();

	// �}�b�v����������
	void MapReMake();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

};

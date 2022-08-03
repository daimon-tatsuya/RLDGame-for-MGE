#pragma once
//**********************************************************
//
//		RogueLikeDungeon�N���X
//
//**********************************************************
#include <vector>
#include "Liblary/Systems/Math.h"

//�Q�l����
//https://qiita.com/gis/items/f760a1fb27aa4c3659cf
//https://qiita.com/gis/items/b9cf998db63b465d9f50
//http://koyoru1214.blog.fc2.com/blog-entry-7.html
//http://koyoru1214.blog.fc2.com/blog-entry-13.html

// �O���[�o���ϐ�
/*�}�b�v�n�f�[�^*/

//XY���ꂼ��ő�48�ɂ���
//49�ȏ�̒l��fps���ቺ������
static int	MAP_SIZE_Y = 48;	// �}�b�v�c�T�C�Y
static int	MAP_SIZE_X = 48;   // �}�b�v���T�C�Y
static int	MAP_SIZE = MAP_SIZE_Y * MAP_SIZE_X;

const static int ROOM_MAX = 8;	// �����̐�
const static int OBJECT_MAX = 40;	// �I�u�W�F�N�g�̏o���ő�l
const static int	 ONE = 1; // DXlib��GetRand�֐����g��Ȃ��ꍇ+1����K�v������
const static int CELL_SIZE = 2;	// world���W�ł̏��ڂ̑傫��

// �}�b�v�̏��̐��^
struct DungeonMapRole final
{
	// ��������镔���̐� (���m�Ɍ����Ɛ����������̐�)
	size_t division_count_min = 4; // �}�b�v�̋敪���ŏ���
	size_t division_count_rand = 4; // �}�b�v�̋敪�����ɉ��Z����ő�l
	size_t division_count_max = division_count_rand + division_count_min; // �}�b�v�̋敪���ő�

	// ��������镔���̃T�C�Y
	size_t room_length_minX = 5;  // ������X���W�̍ŏ��T�C�Y
	size_t room_length_minY = 5;  // ������Y���W�̍ŏ��T�C�Y
	size_t room_length_randX = 5; // ������X���W�̃T�C�Y���Z
	size_t room_length_randY = 5; // ������Y���W�̃T�C�Y���Z

	//	�ۑ��������
	size_t map_division_count{};						// �}�b�v�̋敪���� (�����̌�) 0~n�܂ł̕���ID
	size_t map_division[ROOM_MAX][4]{};		// �}�b�v�̋�� [����ID][Y�I�_ , X�I�_ , Y�n�_ , X�n�_]
	size_t map_room_id[ROOM_MAX]{};			// ������ID
	size_t map_room_count{};							// �����̐�
	size_t map_room[ROOM_MAX][4]{};			// �}�b�v�̕��� [����ID][Y�I�_ , X�I�_ , Y�n�_ , X�n�_]
	size_t map_road[ROOM_MAX][4]{};			// �}�b�v�̒ʘH [����ID(�O)] [�q�����̕���ID(��) , (0:Y���W , 1:X���W) , (�O)���̒ʘH�̈ʒu , (��)���̒ʘH�̈ʒu]
	size_t map_room_area[ROOM_MAX]{};		// �����̖ʐ�
};

///<summary>
/// �}�b�v���̑���
///</summary>
///<para>
/// ���̃I�u�W�F�N�g�ʘH��菬�����l�ɂ���
///</para>
enum class Attribute
{
	Wall = 0,	// 0:��
	Floor,		// 1:��
	Exit,			// 2:�K�i
	Room,		// 3:����
	Road,		// 4:�ʘH
	Player,		// 5:�v���C���[
	Enemy,		// 6:�G
	// 7 : �A�C�e��
	// 8 : �
	// 9 : �A�C�e���̏�̃v���C���[
	// 10 : �A�C�e���̏�̓G
	// 11 : �n�`�̏�̃v���C���[
	// 12 :  �n�`�̏�̓G
	EndAttribute
};

// ���������\�ȃ}�b�v
struct RogueLikeMap final
{

	RogueLikeMap(const size_t var) :map_data(var) {}
	RogueLikeMap() = default;

	size_t map_data = static_cast<size_t>(Attribute::Wall);// �}�X�̑������

	// �ʘH�̓�����������łȂ����̃t���O
	bool is_road_entrance = false; // false:�ʘH �Atrue:�ʘH�̓����

	// �v���[���[��G�A�A�C�e��������Ă��Ă����ݓI�ɕ�����悤����t���O
	bool is_room = false; // false:�ʘH�Atrue:�����@���ǂ͕����ł͂Ȃ�����(false�̂܂�)�Ƃ���B
	//\�܂��A�V���ɑ�����ꍇ�͂��̃t���O�������āAenum class �Ŕ��ʂ���悤�ɂ���
};



// ����̃}�b�v�f�[�^������
class  RogueLikeDungeon final
{
private:

	std::vector<std::vector<RogueLikeMap>> map_role{};// �}�b�v�����i�[����R���e�i

	std::vector<DirectX::XMINT2>  rooms_center{};// �����̒��S���i�[����R���e�i

	std::vector<DirectX::XMINT2>  roads{};// �ʘH�̈ʒu���i�[����R���e�i

public:

	enum class  Road : int//  �}�b�v�̒ʘH�̎�
	{
		Axis_Y = 0,
		Axis_X
	};

	DungeonMapRole dungeon_map_role{};// �}�b�v�\�����

	size_t map_room_player{};	// �v���C���[�̂��镔���̔ԍ�

	//�㏑���h�~�ۑ��p
	DirectX::XMINT2 player_pos{};
	DirectX::XMINT2 stairs_pos{};

private:

	// �}�b�v�̃T�C�Y������������
	void InitializeMapSize();


	// �}�b�v�������s��
	bool MakeMap();

	/// <summary>
	/// �}�b�v�������ƂɃ}�b�v��̕�����object�̈ʒu��ݒ肷��
	/// </summary>
	/// <param name="id">�}�b�v���object�ɕt�^����id</param>
	/// <returns></returns>
	void SetObjectPos(int id);

	// �}�b�v�������
	void ClearMap();


public:

	// �B��̃C���X�^���X�擾
	static RogueLikeDungeon& Instance()
	{
		// �C���X�^���X�ݒ�
		static RogueLikeDungeon instance;
		return instance;
	}

	// �R���X�g���N�^
	RogueLikeDungeon();

	// �f�X�g���N�^
	~RogueLikeDungeon();

	// �}�b�v�����X�V����
	void UpdateMapRole();

	// �V�K�_���W�����쐬���s��
	void MakeDungeon();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI() const;

	//	�}�b�v���̎擾
	std::vector<std::vector<RogueLikeMap>> GetMapRole() { return  map_role; }

	// �ʘH�̃R���e�i���擾
	std::vector<DirectX::XMINT2> GetRoads() { return roads; }

	// �ʘH�̃R���e�i���擾
	std::vector<DirectX::XMINT2> GetRoomsCenter() { return rooms_center; }

	// �����̔ԍ����擾
	//�I�u�W�F�N�g�̃}�b�v�ʒu���𕔉�XY���ꂼ��̂̏I�_�n�_�͈͓̔��������炻�̕����̔ԍ���Ԃ�

	/// <summary>
	/// �����̔ԍ����擾
	/// </summary>
	/// <para>�I�u�W�F�N�g�̃}�b�v�ʒu���𕔉�XY���ꂼ��̂̏I�_�n�_�͈͓̔��������炻�̕����̔ԍ���Ԃ�</para>
	/// <param name="object_map_pos">object�̃}�b�v��̈ʒu</param>
	/// <returns>�����̔ԍ�(�����ɂ��Ȃ��Ȃ�-1	)</returns>
	int GetLocateRoomNumber(DirectX::XMINT2 object_map_pos);
};

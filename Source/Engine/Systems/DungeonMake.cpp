
#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#include <time.h>

#include "Engine/Systems/DungeonMake.h"

#include "Engine/Systems/CharacterManager.h"


RogueLikeDungeon::RogueLikeDungeon()
{
	srand(static_cast<unsigned int>(time(nullptr)));

}
RogueLikeDungeon::~RogueLikeDungeon()
{
	map_role.clear();
}
void RogueLikeDungeon::InitializeMapSize()
{
	map_role.resize(MapSize_Y, std::vector<RogueLikeMap>(MapSize_X, 0));
}
void RogueLikeDungeon::UpdateMapRolePlayer()
{
	//�X�V�O�̃v���C���[�̃f�[�^�̏�������
	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//�������v���C���[�Ȃ�
			if (map_role[y][x].map_data == 2)
			{
				//�����ɃA�C�e����㩂���������4,5
				//���ɏ�������
				map_role[y][x].map_data = 1;
			}
		}
	}

	//�X�V
	CharacterManager& character_manager = CharacterManager::Instance();

	Character* player = character_manager.GetPlayer();

	DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(player->GetPosition().x / Cell_Size, player->GetPosition().z / Cell_Size);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

	//�X�V��̃v���C���[�̃f�[�^�̏�������
	map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x)].map_data = 2;

}

void RogueLikeDungeon::UpdateMapRoleEnemis()
{
	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//�������G�Ȃ�
			if (map_role[y][x].map_data == 3)
			{
				//�����ɃA�C�e����㩂���������4,5
				//���ɏ�������
				map_role[y][x].map_data = 1;
			}
		}
	}
	//�X�V
	CharacterManager& character_manager = CharacterManager::Instance();
	for (auto& enemy : character_manager.GetEnemis())
	{
		DirectX::XMFLOAT2 enemy_pos = DirectX::XMFLOAT2(enemy->GetPosition().x / Cell_Size, enemy->GetPosition().z / Cell_Size);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

	//�X�V��̃v���C���[�̃f�[�^�̏�������
		map_role[static_cast<size_t>(enemy_pos.y)][static_cast<size_t>(enemy_pos.x)].map_data = 3;
	}

}

void RogueLikeDungeon::DungeonMake(DungeonMapRole* dungeon_map_role)
{
	for (int i = 0; i < Mob_Max; i++)
	{
		mobs[i].position = { 0 ,0 };
	}
	MapMake(dungeon_map_role);

	for (int i = 0; i < Mob_Max; i++)
	{
		MobMake(dungeon_map_role, map_role, i);
	}
}

void RogueLikeDungeon::MapClear()
{
	map_role.clear();

}

void RogueLikeDungeon::MapReMake(DungeonMapRole* dungeon_map_role)
{
	MapClear();
	InitializeMapSize();
	DungeonMake(dungeon_map_role);
}

bool RogueLikeDungeon::MobMake(DungeonMapRole* const dungeon_map_role, std::vector<std::vector<RogueLikeMap>>& map_role, int id)
{
	//Mob��ݒu����
	int random_room_id = dungeon_map_role->map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room_count)))]; //�}�b�v��̕����������_���Ɏw�肷��

	int random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][0] - dungeon_map_role->map_room[random_room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
	int position_y = static_cast<int>(dungeon_map_role->map_room[random_room_id][2]) + random_pos_y; //�}�b�v��̕����̃����_����Y���W

	int random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][1] - dungeon_map_role->map_room[random_room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
	int position_x = static_cast<int>(dungeon_map_role->map_room[random_room_id][3]) + random_pos_x; //�}�b�v��̕����̃����_����X���W

	//id��0�Ԗڂ�player
	if (id == 0)
	{
		//	�v���C���[�̈ʒu
		map_role[position_y][position_x].map_data = 2;
		mobs[id].position = { static_cast<float>(position_x),static_cast<float>(position_y) };
		dungeon_map_role->map_room_player[random_room_id] = 1; //�����Ƀv���C���[������(�N��)
	}
	else//id!=0
	{
		while (((mobs[0].position.y == position_y) && (mobs[0].position.x == position_x)))//�v���C���[�Əd�Ȃ��Ă���Ȃ�
		{
			//Mob��ݒu����
			random_room_id = dungeon_map_role->map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room_count)))]; //�}�b�v��̕����������_���Ɏw�肷��

			random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][0] - dungeon_map_role->map_room[random_room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
			position_y = static_cast<int>(dungeon_map_role->map_room[random_room_id][2]) + random_pos_y; //�}�b�v��̕����̃����_����Y���W

			random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][1] - dungeon_map_role->map_room[random_room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
			position_x = static_cast<int>(dungeon_map_role->map_room[random_room_id][3]) + random_pos_x; //�}�b�v��̕����̃����_����X���W
		}
		//�G�̈ʒu
		map_role[position_y][position_x].map_data = 3;
		mobs[id].position = { static_cast<float>(position_x),static_cast<float>(position_y) };
	}
	//mobs[id].id = 1;

	////����mob�Əd�Ȃ��Ă��������
	//for (int i = 0; i < Mob_Max; i++)
	//{
	//	//���ł��ǂł��Ȃ�
	//	if (map_role[position_y][position_x].map_data < 1)
	//	{
	//		mobs[id].id = 0;
	//		break;
	//	}
	//}
	//�G���[�`�F�b�N
	if (!mobs) { return false; }

	return true;
}

bool RogueLikeDungeon::MapMake(DungeonMapRole* const dungeon_map_role/*, std::vector<std::vector<RogueLikeMap>>& map_role*/)
{
	if (map_role.size() == 0 || map_role.front().size() == 0)
	{
		return false;
	}

	for (size_t i = 0; i < map_role.size(); i++)
	{
		for (size_t j = 0; j < map_role[i].size(); j++)
		{
			map_role[i][j].map_data = 0;
		}
	}
	//�}�b�v�̋敪���� (�����̌�) 0~n�܂ł̕���ID
	dungeon_map_role->map_division_count = dungeon_map_role->division_count_min +
		static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->division_count_rand + ONE)));

	if (dungeon_map_role->map_division_count > 9)
	{
		return true;
	}

	for (int i = 0; i < dungeon_map_role->map_division_count; i++)
	{
		dungeon_map_role->map_room_area[i] = 0;
	}

	dungeon_map_role->map_division[0][0] = (map_role.size() - 1); //�}�b�v�̋敪�������T�C�YY�I�_ (�}�b�v�̑傫��Y��)
	dungeon_map_role->map_division[0][1] = (map_role.front().size() - 1); //�}�b�v�̋敪�������T�C�YX�I�_ (�}�b�v�̑傫��X��)
	dungeon_map_role->map_division[0][2] = 1; //�}�b�v�̋敪�������T�C�YX�n�_ (�}�b�v�̑傫��Y��)
	dungeon_map_role->map_division[0][3] = 1; //�}�b�v�̋敪�������T�C�YY�n�_ (�}�b�v�̑傫��X��)

	dungeon_map_role->map_road[0][0] = 255;
	dungeon_map_role->map_road[0][1] = 255;

	/*�}�b�v���敪�����Ă�������(���𕪊����鏈��)*/
	size_t division_after;
	int count; //(0:Y, 1:X) Y���ŕ����邩X���ŕ����邩���߂�

	for (size_t i = 1; i < dungeon_map_role->map_division_count; i++)
	{
		//���܂ō�����敪���������_���Ɏw��(�w�肵����������ɋ敪������)
		division_after = static_cast<size_t>(Math::RandomInt(static_cast<int>(i)));

		//�w�肵������Y��X�̒����ɂ���āA����������������߂�(�����ق��𕪊�����)
		if (dungeon_map_role->map_division[division_after][0] - dungeon_map_role->map_division[division_after][2] > dungeon_map_role->map_division[division_after][1] - dungeon_map_role->map_division[division_after][3])
		{
			count = static_cast<int>(Road::Axis_Y);
		}
		else
		{
			count = static_cast<int>(Road::Axis_X);
		}

		if (dungeon_map_role->map_division[division_after][count] - dungeon_map_role->map_division[division_after][count + 2] < dungeon_map_role->division_count_rand * 2 + 8)
		{
			size_t k = 0;
			for (size_t j = 1; j < dungeon_map_role->map_division_count; j++)
			{
				if (dungeon_map_role->map_division[j][0] - dungeon_map_role->map_division[j][2] > k)
				{
					k = dungeon_map_role->map_division[j][0] - dungeon_map_role->map_division[j][2];
					division_after = j;
					count = static_cast<int>(Road::Axis_Y);
				}
				if (dungeon_map_role->map_division[j][1] - dungeon_map_role->map_division[j][3] > k)
				{
					k = dungeon_map_role->map_division[j][1] - dungeon_map_role->map_division[j][3];
					division_after = j;
					count = static_cast<int>(Road::Axis_X);
				}
			}
		}

		dungeon_map_role->map_road[i][0] = division_after;
		dungeon_map_role->map_road[i][1] = count;

		for (size_t j = 1; j < i; j++)
		{
			if (dungeon_map_role->map_road[j][0] == division_after)
			{
				dungeon_map_role->map_road[j][0] = i;
			}
		}
		//count���̐ݒ�

		//0.���̉E�[(iR)�̍��W(divAfter*R/3~2divAfter*R/3)
		dungeon_map_role->map_division[i][count] = dungeon_map_role->map_division[division_after][count + 2] +
			(dungeon_map_role->map_division[division_after][count] - dungeon_map_role->map_division[division_after][count + 2]) / 3 +
			static_cast<size_t>(Math::RandomInt(((dungeon_map_role->map_division[division_after][count] - dungeon_map_role->map_division[division_after][count + 2]) / 3)));

		dungeon_map_role->map_division[i][count + 2] = dungeon_map_role->map_division[division_after][count + 2]; //0.���̍��[(iL)�̍��W(divAfterL)
		dungeon_map_role->map_division[division_after][count + 2] = dungeon_map_role->map_division[i][count]; //divAfter���̍��[(divAfterL)�̍��W(iR)

		//count�Ƃ͋t�̎��̐ݒ�
		dungeon_map_role->map_division[i][abs(count - 1)] = dungeon_map_role->map_division[division_after][abs(count - 1)]; //���̉E�[(iR)�̍��W(divAfterR)
		dungeon_map_role->map_division[i][abs(count - 1) + 2] = dungeon_map_role->map_division[division_after][abs(count - 1) + 2]; //���̍��[(iL)�̍��W(divAfterL)
	}

	/*�����𐶐����鏈��*/
	for (size_t i = 0; i < dungeon_map_role->map_division_count; i++)//�敪��
	{
		dungeon_map_role->map_room_player[i] = 0;//�v���C���[�N��������
		dungeon_map_role->map_room[i][2] = dungeon_map_role->map_division[i][2]; //�敪��Y�n�_���}�b�vY�n�_�֑��
		dungeon_map_role->map_room[i][3] = dungeon_map_role->map_division[i][3]; //�敪��X�n�_���}�b�vX�n�_�֑��

		//Y���W�̕����̒������w��
		dungeon_map_role->map_room[i][0] = dungeon_map_role->map_division[i][2] + dungeon_map_role->division_count_rand + static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->room_length_randX + ONE)));
		if (dungeon_map_role->map_division[i][0] - dungeon_map_role->map_division[i][2] < dungeon_map_role->map_room[i][0] - dungeon_map_role->map_room[i][2] + 5)
		{
			dungeon_map_role->map_room[i][0] = dungeon_map_role->map_division[i][0] - 4;
			if (dungeon_map_role->map_division[i][0] - dungeon_map_role->map_division[i][2] < dungeon_map_role->map_room[i][0] - dungeon_map_role->map_room[i][2] + 5)
			{
				dungeon_map_role->map_room[i][0] = dungeon_map_role->map_division[i][2] + 1;
			}
		}

		dungeon_map_role->map_room[i][1] = dungeon_map_role->map_division[i][3] + dungeon_map_role->room_length_minY + static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->room_length_randY + ONE)));
		if (dungeon_map_role->map_division[i][1] - dungeon_map_role->map_division[i][3] < dungeon_map_role->map_room[i][1] - dungeon_map_role->map_room[i][3] + 5)
		{
			dungeon_map_role->map_room[i][1] = dungeon_map_role->map_division[i][1] - 4;
			if (dungeon_map_role->map_division[i][1] - dungeon_map_role->map_division[i][3] < dungeon_map_role->map_room[i][1] - dungeon_map_role->map_room[i][3] + 5)
			{
				dungeon_map_role->map_room[i][1] = dungeon_map_role->map_division[i][3] + 1;
			}
		}

		if (dungeon_map_role->map_room[i][0] - dungeon_map_role->map_division[i][2] <= 1 || dungeon_map_role->map_room[i][1] - dungeon_map_role->map_division[i][3] <= 1)
		{
			dungeon_map_role->map_room[i][0] = dungeon_map_role->map_division[i][2] + 1;
			dungeon_map_role->map_room[i][1] = dungeon_map_role->map_division[i][3] + 1;
		}
		//Todo �}�W�b�N�i���o�[
		size_t l = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_division[i][0] - dungeon_map_role->map_room[i][0] - 5)) + static_cast<size_t>(2));
		size_t m = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_division[i][1] - dungeon_map_role->map_room[i][1] - 5)) + static_cast<size_t>(2));
		dungeon_map_role->map_room[i][0] += l;
		dungeon_map_role->map_room[i][2] += l;
		dungeon_map_role->map_room[i][1] += m;
		dungeon_map_role->map_room[i][3] += m;

		for (size_t j = dungeon_map_role->map_room[i][2]; j < dungeon_map_role->map_room[i][0]; j++)
		{
			for (size_t k = dungeon_map_role->map_room[i][3]; k < dungeon_map_role->map_room[i][1]; k++)
			{
				map_role[j][k].map_data = 1;
				dungeon_map_role->map_room_area[i]++;
			}
		}

		if (dungeon_map_role->map_room_area[i] > 1)
		{
			dungeon_map_role->map_room_count++;
			dungeon_map_role->map_room_id[dungeon_map_role->map_room_count] = i;
		}
	}

	/*�ʘH�𐶐����鏈��*/
	/*�ʘH�͂Q�����Ԃׂ̍����̂��Ƃ��w���B
	�ʘH����邽�߂ɂQ���������ꂼ��O(Before)�ƌ�(After)�ŕ�����B
	for���őS�Ă̕������`�F�b�N���A�O��̕������q���ʘH�����B
	�܂��A�O�̒ʘH�����A���Ɍ�̒ʘH�����B
	�Ō�ɑO�ƌ�̒ʘH���q����B
	*/
	size_t room_after;
	for (size_t room_before = 0; room_before < dungeon_map_role->map_division_count; room_before++)
	{
		room_after = dungeon_map_role->map_road[room_before][0];

		//Y���W�̒ʘH
		switch (dungeon_map_role->map_road[room_before][1])
		{
			case static_cast<size_t>(Road::Axis_Y) :
				//�O���̒ʘH�̈ʒu
				dungeon_map_role->map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_before][1] - dungeon_map_role->map_room[room_before][3] - 2)));
				//�㑤�̒ʘH�̈ʒu
				dungeon_map_role->map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_after][1] - dungeon_map_role->map_room[room_after][3] - 2)));
				//�O�̒ʘH
				for (size_t j = dungeon_map_role->map_room[room_before][0]; j < dungeon_map_role->map_division[room_before][0]; j++)
				{
					map_role[j][dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][3]].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ���
				}
				//��̒ʘH
				for (size_t j = dungeon_map_role->map_division[room_after][2]; j < dungeon_map_role->map_room[room_after][2]; j++)
				{
					map_role[j][dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][3]].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ���
				}
				//�ʘH���Ȃ�
				for (size_t j = dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][3]; j <= dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][3]; j++)
				{
					map_role[dungeon_map_role->map_division[room_before][0]][j].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ��� 2����5(�ォ�牺)
				}
				for (size_t j = dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][3]; j <= dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][3]; j++)
				{
					map_role[dungeon_map_role->map_division[room_before][0]][j].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ��� 5����2(�������)
				}
				break;

				case static_cast<size_t>(Road::Axis_X) :
					//�O���̒ʘH�̈ʒu
					dungeon_map_role->map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_before][0] - dungeon_map_role->map_room[room_before][2] - ONE)));
					//�㑤�̒ʘH�̈ʒu
					dungeon_map_role->map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_after][0] - dungeon_map_role->map_room[room_after][2] - ONE)));
					//�O�̒ʘH
					for (size_t j = dungeon_map_role->map_room[room_before][1]; j < dungeon_map_role->map_division[room_before][1]; j++)
					{
						map_role[dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][2]][j].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ���
					}
					//��̒ʘH
					for (size_t j = dungeon_map_role->map_division[room_after][3]; j < dungeon_map_role->map_room[room_after][3]; j++)
					{
						map_role[dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][2]][j].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ���
					}
					//�ʘH���Ȃ�
					for (size_t j = dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][2]; j <= dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][2]; j++)
					{
						map_role[j][dungeon_map_role->map_division[room_before][1]].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ���
					}
					for (size_t j = dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][2]; j <= dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][2]; j++)
					{
						map_role[j][dungeon_map_role->map_division[room_before][1]].map_data = 1; //�ʘH���}�b�v�`�b�v�ɐ���
					}
					break;
		}
	}
	//�G���[�`�F�b�N
	if (!dungeon_map_role->map_division_count)
	{
		return false;
	}

	return true;
}
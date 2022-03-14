//**********************************************************
//
//		RogueLikeDungeon�N���X
//
//**********************************************************

//! xy���ő�ŏ��l�𒴂��Ȃ��悤��xy���O�܂��͍ő�l�̎��A�ύX����Ȃ�������ݒ肷��

#include <ctime>

#include "Engine/AI/DungeonMake.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine//Systems/Character.h"
#include "Engine/Systems/ImGuiRenderer.h"

RogueLikeDungeon::RogueLikeDungeon()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

RogueLikeDungeon::~RogueLikeDungeon()
{
	//	���
	map_role.clear();
}

void RogueLikeDungeon::InitializeMapSize()
{
	//�Đݒ�
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
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Player))
			{
				//�ǂȂ疳��(����A�ǂ𖳎�����ړ����ǉ������Ȃ�ύX����)
				if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Wall))
				{
					continue;
				}
				//\�����ɃA�C�e����㩂���������4,5
				//���ɏ�������
				if (map_role[y][x].is_room == false)//���̑������ʘH
				{
					map_role[y][x].map_data = static_cast<size_t>(Attribute::Road);
				}
				else if (map_role[y][x].is_room == true)//���̑���������
				{
					map_role[y][x].map_data = static_cast<size_t>(Attribute::Room);
				}
			}
		}
	}

	//�X�V

	const Character* player = CharacterManager::Instance().GetPlayer();

	const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(player->GetPosition().x / CellSize, player->GetPosition().z / CellSize);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

	//�X�V��̃v���C���[�̃f�[�^�̏�������
	map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x)].map_data = static_cast<size_t>(Attribute::Player);
}

void RogueLikeDungeon::UpdateMapRoleEnemies()
{
	//! �ȉ���{}���֐����܂��́AUpdateMapRolePlayer()�Ƃ܂Ƃ߂�
	//? {
	//��x�㏑��
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//�ǂȂ疳��(����A�ǂ𖳎�����G���ړ����ǉ������Ȃ�ύX����)
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				continue;
			}
			//\�����ɃA�C�e����㩂���������4,5
			//���ɏ�������

			if (map_role[y][x].is_room == false)//���̑������ʘH
			{
				map_role[y][x].map_data = static_cast<size_t>(Attribute::Road);
			}
			else if (map_role[y][x].is_room == true)//���̑���������
			{
				map_role[y][x].map_data = static_cast<size_t>(Attribute::Room);
			}
		}
	}
	//?}

	//�X�V
	const CharacterManager& character_manager = CharacterManager::Instance();
	for (const auto& enemy : character_manager.GetCharacters())
	{
		if (enemy->GetId() < static_cast<int>(Meta::Identity::Enemy))//ID���G�̒l�łȂ��Ȃ�ȉ��Ȃ�
		{
			continue;
		}
		const DirectX::XMFLOAT2 enemy_pos = DirectX::XMFLOAT2(enemy->GetPosition().x / CellSize, enemy->GetPosition().z / CellSize);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

	//�X�V��̃v���C���[�̃f�[�^�̏�������
		map_role[static_cast<size_t>(enemy_pos.y)][static_cast<size_t>(enemy_pos.x)].map_data = static_cast<int>(Attribute::Enemy);
	}
}

void RogueLikeDungeon::DungeonMake()
{
	for (auto& mob : mobs)
	{
		mob.position = { 0 ,0 };
	}

	MapMake();

	for (int i = 0; i < MobMax; i++)
	{
		//�����_���ɓG��
		MobMake(i);
	}
}

void RogueLikeDungeon::MapClear()
{
	//	���
	map_role.clear();
}

void RogueLikeDungeon::MapReMake()
{
	//	���
	MapClear();

	//�T�C�Y�̍Đݒ�
	InitializeMapSize();

	//�_���W��������
	DungeonMake();
}

void RogueLikeDungeon::DrawDebugGUI() const
{
	ImGui::SetNextWindowPos(ImVec2(0, 350), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 350), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("MapInformation ", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Text("MapSize:%d \nMapSize_Y:%d MapSize_X:%d", MapSize, MapSize_Y, MapSize_X);
		ImGui::Text("MapRoomCount:%d", static_cast<int>(dungeon_map_role.map_room_count));//�����̐�
		ImGui::Text("MapDivisionCount:%d", static_cast<int>(dungeon_map_role.map_division_count));//��������邽�߂ɕ���������(�����̐��ƈ�v���Ȃ��ꍇ�ʘH���˂���Ă���)

		if (ImGui::CollapsingHeader("AttributeInfo", ImGuiTreeNodeFlags_OpenOnArrow))
		{
			ImGui::Text("Wall:0");
			ImGui::Text("Floor:1");
			ImGui::Text("Room:2");
			ImGui::Text("Road:3");
			ImGui::Text("Player:4");
			ImGui::Text("Enemy:5");
			ImGui::Text("Exit:6");
			/*Attribute
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
			// 11 :  �n�`�̏�̓G*/
		}
	}
	ImGui::End();
}

bool RogueLikeDungeon::MobMake(const int id)
{
	//Mob��ݒu����
	int random_room_id = static_cast<int>(dungeon_map_role.map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room_count)))]); //�}�b�v��̕����������_���Ɏw�肷��

	int random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][0] - dungeon_map_role.map_room[random_room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
	int position_y = static_cast<int>(dungeon_map_role.map_room[random_room_id][2]) + random_pos_y; //�}�b�v��̕����̃����_����Y���W

	int random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][1] - dungeon_map_role.map_room[random_room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
	int position_x = static_cast<int>(dungeon_map_role.map_room[random_room_id][3]) + random_pos_x; //�}�b�v��̕����̃����_����X���W

	//�d�Ȃ��Ă����ꍇ��������
	if (id == 0)	//id��0�Ԗڂ̓v���C���[�Ƃ��Ă���
	{
		//	�v���C���[�̈ʒu
		map_role[position_y][position_x].map_data = static_cast<size_t>(Attribute::Player);
		mobs[id].position = { static_cast<float>(position_x),static_cast<float>(position_y) };
		//dungeon_map_role.map_room_player[random_room_id] = 1; //�����Ƀv���C���[������
		map_room_player = random_room_id; //�����Ƀv���C���[������
	}
	else
	{
		while (mobs[0].position.y == position_y && mobs[0].position.x == position_x)//�v���C���[�Əd�Ȃ��Ă���Ȃ�
		{//�v���C���[�Əd�Ȃ��Ă��Ȃ������炱�̃��[�v�𔲂���
			//Mob��ݒu����
			random_room_id = static_cast<int>(dungeon_map_role.map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room_count)))]); //�}�b�v��̕����������_���Ɏw�肷��

			random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][0] - dungeon_map_role.map_room[random_room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
			position_y = static_cast<int>(dungeon_map_role.map_room[random_room_id][2]) + random_pos_y; //�}�b�v��̕����̃����_����Y���W

			random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][1] - dungeon_map_role.map_room[random_room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
			position_x = static_cast<int>(dungeon_map_role.map_room[random_room_id][3]) + random_pos_x; //�}�b�v��̕����̃����_����X���W
		}
		//�G�̈ʒu
		map_role[position_y][position_x].map_data = static_cast<size_t>(Attribute::Enemy);
		mobs[id].position = { static_cast<float>(position_x),static_cast<float>(position_y) };
		mobs[id].id = 1;
	}

	//�ǂƏd�Ȃ��Ă��������
	for (int i = 0; i < MobMax; i++)
	{
		//�ǂ̏�ɐ�������Ă�����
		if (map_role[position_y][position_x].map_data < 1)
		{
			mobs[id].id = 0;
			break;
		}
	}

	if (!mobs)
	{
		return false;
	}

	return true;
}

bool RogueLikeDungeon::MapMake()
{
	if (map_role.empty() || map_role.front().empty())
	{
		return false;
	}

	//�}�b�v��ǂŖ��߂�
	for (auto& i : map_role)
	{
		for (auto& j : i)
		{
			j.map_data = static_cast<size_t>(Attribute::Wall);
			j.is_room = false;
			j.is_road_entrance = false;
		}
	}
	//�}�b�v�̋敪����
	dungeon_map_role.map_division_count = dungeon_map_role.division_count_min +
		static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.division_count_rand + ONE)));

	if (dungeon_map_role.map_division_count > dungeon_map_role.division_count_max)
	{
		return false;
	}

	//�����̏�����
	for (int i = 0; i < dungeon_map_role.map_division_count; i++)
	{
		dungeon_map_role.map_room_area[i] = 0;
	}

	dungeon_map_role.map_division[0][0] = (map_role.size() - 1); //�}�b�v�̋敪�������T�C�YY�I�_ (�}�b�v�̑傫��Y��)
	dungeon_map_role.map_division[0][1] = (map_role.front().size() - 1); //�}�b�v�̋敪�������T�C�YX�I�_ (�}�b�v�̑傫��X��)
	dungeon_map_role.map_division[0][2] = 1; //�}�b�v�̋敪�������T�C�YX�n�_ (�}�b�v�̑傫��Y��)
	dungeon_map_role.map_division[0][3] = 1; //�}�b�v�̋敪�������T�C�YY�n�_ (�}�b�v�̑傫��X��)

	dungeon_map_role.map_road[0][0] = 255;
	dungeon_map_role.map_road[0][1] = 255;

	/*�}�b�v���敪�����Ă�������(���ɕ������鏈��)*/

	size_t division_after{};
	int count{}; //(0:Y, 1:X) Y���ŕ����邩X���ŕ����邩���߂�

	for (size_t i = 1; i < dungeon_map_role.map_division_count; i++)
	{
		//���܂ō�����敪���������_���Ɏw��(�w�肵����������ɋ敪������)
		division_after = static_cast<size_t>(Math::RandomInt(static_cast<int>(i)));

		//�w�肵������Y��X�̒����ɂ���āA����������������߂�(�����ق��𕪊�����)
		if (dungeon_map_role.map_division[division_after][0] - dungeon_map_role.map_division[division_after][2] >
			dungeon_map_role.map_division[division_after][1] - dungeon_map_role.map_division[division_after][3])
		{
			count = static_cast<int>(Road::Axis_Y);
		}
		else
		{
			count = static_cast<int>(Road::Axis_X);
		}

		//����������������
		if (dungeon_map_role.map_division[division_after][count] - dungeon_map_role.map_division[division_after][count + 2] <
			dungeon_map_role.division_count_rand * 2 + RoomMax)
		{
			size_t k = 0;
			for (size_t j = 1; j < dungeon_map_role.map_division_count; j++)
			{
				//�}�b�v����Y�@ Y�I�_ - Y�n�_
				if (dungeon_map_role.map_division[j][0] - dungeon_map_role.map_division[j][2] > k)
				{
					k = dungeon_map_role.map_division[j][0] - dungeon_map_role.map_division[j][2];
					division_after = j;
					count = static_cast<int>(Road::Axis_Y);
				}
				//�}�b�v����X�@ X�I�_ - X�n�_
				if (dungeon_map_role.map_division[j][1] - dungeon_map_role.map_division[j][3] > k)
				{
					k = dungeon_map_role.map_division[j][1] - dungeon_map_role.map_division[j][3];
					division_after = j;
					count = static_cast<int>(Road::Axis_X);
				}
			}
		}

		dungeon_map_role.map_road[i][0] = division_after;
		dungeon_map_role.map_road[i][1] = count;

		for (size_t l = 1; l < i; l++)
		{
			if (dungeon_map_role.map_road[l][0] == division_after)
			{
				dungeon_map_role.map_road[l][0] = i;
			}
		}
		//count���̐ݒ�

		//0.���̉E�[(iR)�̍��W(divAfter*R/3~2divAfter*R/3)
		dungeon_map_role.map_division[i][count] = dungeon_map_role.map_division[division_after][count + 2] +
			(dungeon_map_role.map_division[division_after][count] - dungeon_map_role.map_division[division_after][count + 2]) / 3 +
			static_cast<size_t>
			(Math::RandomInt
			(
				((static_cast<int>(dungeon_map_role.map_division[division_after][count]) - static_cast<int>(dungeon_map_role.map_division[division_after][count + 2])) / 3)
			)
				);

		dungeon_map_role.map_division[i][count + 2] = dungeon_map_role.map_division[division_after][count + 2]; //0.���̍��[(iL)�̍��W(divAfterL)
		dungeon_map_role.map_division[division_after][count + 2] = dungeon_map_role.map_division[i][count]; //divAfter���̍��[(divAfterL)�̍��W(iR)

		//count�Ƃ͋t�̎��̐ݒ�
		dungeon_map_role.map_division[i][abs(count - 1)] = dungeon_map_role.map_division[division_after][abs(count - 1)]; //���̉E�[(iR)�̍��W(divAfterR)
		dungeon_map_role.map_division[i][abs(count - 1) + 2] = dungeon_map_role.map_division[division_after][abs(count - 1) + 2]; //���̍��[(iL)�̍��W(divAfterL)
	}

	/*�����𐶐����鏈��*/
	for (size_t i = 0; i < dungeon_map_role.map_division_count; i++)//�敪��
	{
		map_room_player = 0;//�v���C���[�N��������
		dungeon_map_role.map_room[i][2] = dungeon_map_role.map_division[i][2]; //�敪��Y�n�_���}�b�vY�n�_�֑��
		dungeon_map_role.map_room[i][3] = dungeon_map_role.map_division[i][3]; //�敪��X�n�_���}�b�vX�n�_�֑��

		/*Y���W�̕����̒������w��*/

		//�����ԍ�i��	Y���̏I�_ �� ���ԍ�i��Y���̎n�_+�敪���̃����_���l+1~�C�ӂ̒l(dungeon_map_role.room_length_randY)�܂ł̃����_���l
		dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][2] + dungeon_map_role.room_length_minY
			+ static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.room_length_randY + ONE)));

		//���ԍ�i��Y���̒����������ԍ�i��Y���̒���+5��菬�����Ȃ�
		if (dungeon_map_role.map_division[i][0] - dungeon_map_role.map_division[i][2] < dungeon_map_role.map_room[i][0] - dungeon_map_role.map_room[i][2] + 5)
		{
			//�����ԍ�i��Y���̏I�_�����ԍ�i��Y���I�_-4�ɂ��Ĕ͈͂Ɏ��߂�
			dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][0] - 4;

			if (dungeon_map_role.map_division[i][0] - dungeon_map_role.map_division[i][2] < dungeon_map_role.map_room[i][0] - dungeon_map_role.map_room[i][2] + 5)
			{
				dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][2] + 1;
			}
		}
		/* X���W�̕����̒������w��*/

		//�����ԍ�i��	X���̏I�_ �� ���ԍ�i��X���̎n�_+�敪���̃����_���l+1~�C�ӂ̒l(dungeon_map_role.room_length_randX)�܂ł̃����_���l
		dungeon_map_role.map_room[i][1] = dungeon_map_role.map_division[i][3] + dungeon_map_role.room_length_minX
			+ static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.room_length_randX + ONE)));

		if (dungeon_map_role.map_division[i][1] - dungeon_map_role.map_division[i][3] < dungeon_map_role.map_room[i][1] - dungeon_map_role.map_room[i][3] + 5)
		{
			dungeon_map_role.map_room[i][1] = dungeon_map_role.map_division[i][1] - 4;

			if (dungeon_map_role.map_division[i][1] - dungeon_map_role.map_division[i][3] < dungeon_map_role.map_room[i][1] - dungeon_map_role.map_room[i][3] + 5)
			{
				dungeon_map_role.map_room[i][1] = dungeon_map_role.map_division[i][3] + 1;
			}
		}

		if (dungeon_map_role.map_room[i][0] - dungeon_map_role.map_division[i][2] <= 1 || dungeon_map_role.map_room[i][1] - dungeon_map_role.map_division[i][3] <= 1)
		{
			dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][2] + 1;
			dungeon_map_role.map_room[i][1] = dungeon_map_role.map_division[i][3] + 1;
		}

		const size_t l = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_division[i][0] - dungeon_map_role.map_room[i][0] - 5)) + static_cast<size_t>(2));
		dungeon_map_role.map_room[i][0] += l;
		dungeon_map_role.map_room[i][2] += l;

		const size_t m = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_division[i][1] - dungeon_map_role.map_room[i][1] - 5)) + static_cast<size_t>(2));
		dungeon_map_role.map_room[i][1] += m;
		dungeon_map_role.map_room[i][3] += m;

		//�����̏��̔z�u
		for (size_t j = dungeon_map_role.map_room[i][2]; j < dungeon_map_role.map_room[i][0]; j++)
		{
			for (size_t k = dungeon_map_role.map_room[i][3]; k < dungeon_map_role.map_room[i][1]; k++)
			{
				map_role[j][k].map_data = static_cast<size_t>(Attribute::Room);
				map_role[j][k].is_room = true;
				dungeon_map_role.map_room_area[i]++;
			}
		}

		if (dungeon_map_role.map_room_area[i] > 1)
		{
			dungeon_map_role.map_room_count++;
			dungeon_map_role.map_room_id[dungeon_map_role.map_room_count] = i;
		}
	}

	/*�ʘH�𐶐����鏈��*/
	/*�ʘH�͂Q�����Ԃׂ̍����̂��Ƃ��w���B
	�ʘH����邽�߂ɂQ���������ꂼ��O(Before)�ƌ�(After)�ŕ�����B
	�S�Ă̕������`�F�b�N���A�O��̕������q���ʘH�����B
	�܂��A�O�̒ʘH�����A���Ɍ�̒ʘH�����B
	�Ō�ɑO�ƌ�̒ʘH���q����B
	*/
	//�O�͓������΂�����
	//��͓����q���镔��

	size_t room_after{};
	for (size_t room_before = 0; room_before < dungeon_map_role.map_division_count; room_before++)
	{
		room_after = dungeon_map_role.map_road[room_before][0];/*�q�����̕���ID*/

		bool is_once = false;//for�����A��x�����s����Ƃ̂��߂̃t���O

		//Y���W�̒ʘH
		switch (dungeon_map_role.map_road[room_before][1]/*0:Y���W , 1:X���W*/)
		{
			case static_cast<size_t>(Road::Axis_Y) :

				//�O���̒ʘH�̈ʒu
				dungeon_map_role.map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_before][1] - dungeon_map_role.map_room[room_before][3] - 2)));
				//�㑤�̒ʘH�̈ʒu
				dungeon_map_role.map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_after][1] - dungeon_map_role.map_room[room_after][3] - 2)));

				//�O�̒ʘH�𕪊����܂ŐL�΂�
				for (size_t j = dungeon_map_role.map_room[room_before][0]; j < dungeon_map_role.map_division[room_before][0]; j++)
				{
					//�ʘH�̓������ۑ�
					if (is_once == false)
					{
						map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].is_road_entrance = true;
						is_once = true;
					}

					size_t a = dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3];
					map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ�
					map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].is_room = false;//���̃f�[�^�͒ʘH
				}

				is_once = false;

				//��̒ʘH�𕪊����܂ŐL�΂�
				for (size_t j = dungeon_map_role.map_division[room_after][2]; j < dungeon_map_role.map_room[room_after][2]; j++)
				{
					//�ʘH�̓������ۑ�
					if (is_once == false)
					{
						map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].is_road_entrance = true;
						is_once = true;
					}

					map_role[j][dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ�
					map_role[j][dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]].is_room = false;//���̃f�[�^�͒ʘH
				}

				is_once = false;

				//�ʘH���Ȃ�(��������ɐL�΂�)
				for (size_t j = dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]; j <= dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]; j++)
				{
					map_role[dungeon_map_role.map_division[room_before][0]][j].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ� 2����5(�ォ�牺)
					map_role[dungeon_map_role.map_division[room_before][0]][j].is_room = false;//���̃f�[�^�͒ʘH
				}

				for (size_t j = dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]; j <= dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]; j++)
				{
					map_role[dungeon_map_role.map_division[room_before][0]][j].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ� 5����2(�������)
					map_role[dungeon_map_role.map_division[room_before][0]][j].is_room = false;//���̃f�[�^�͒ʘH
				}
				break;

				case static_cast<size_t>(Road::Axis_X) :

					//�O���̒ʘH�̈ʒu
					dungeon_map_role.map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_before][0] - dungeon_map_role.map_room[room_before][2] - ONE)));

					//�㑤�̒ʘH�̈ʒu
					dungeon_map_role.map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_after][0] - dungeon_map_role.map_room[room_after][2] - ONE)));

					//�O�̒ʘH�𕪊����܂ŐL�΂�
					for (size_t j = dungeon_map_role.map_room[room_before][1]; j < dungeon_map_role.map_division[room_before][1]; j++)
					{
						//�ʘH�̓������ۑ�
						if (is_once == false)
						{
							map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].is_road_entrance = true;
							is_once = true;
						}

						map_role[dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]][j].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ�
						map_role[dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]][j].is_room = false;//���̃f�[�^�͒ʘH
					}

					is_once = false;

					//��̒ʘH�𕪊����܂ŐL�΂�
					for (size_t j = dungeon_map_role.map_division[room_after][3]; j < dungeon_map_role.map_room[room_after][3]; j++)
					{
						//�ʘH�̓������ۑ�
						if (is_once == false)
						{
							map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].is_road_entrance = true;
							is_once = true;
						}

						map_role[dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]][j].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ�
						map_role[dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]][j].is_room = false;//���̃f�[�^�͒ʘH
					}

					//�ʘH���Ȃ�
					for (size_t j = dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]; j <= dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]; j++)
					{
						map_role[j][dungeon_map_role.map_division[room_before][1]].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ�
						map_role[j][dungeon_map_role.map_division[room_before][1]].is_room = false;//���̃f�[�^�͒ʘH
					}
					for (size_t j = dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]; j <= dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]; j++)
					{
						map_role[j][dungeon_map_role.map_division[room_before][1]].map_data = static_cast<size_t>(Attribute::Road); //�ʘH�̑����ɐݒ�
						map_role[j][dungeon_map_role.map_division[room_before][1]].is_room = false;//���̃f�[�^�͒ʘH
					}
					break;

				default:
					break;
		}
	}
	//�G���[�`�F�b�N
	if (!dungeon_map_role.map_division_count)
	{
		return false;
	}

	//���̓�����������i�[����
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (map_role[y][x].is_road_entrance == true)
			{
				DirectX::XMINT2 entrance_array = { y,x };
				roads_entrance.emplace_back(entrance_array);
			}
		}
	}
	//�G���[�`�F�b�N

	return true;
}
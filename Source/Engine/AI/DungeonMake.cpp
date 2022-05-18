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
#include "Engine/Systems/Math.h"


//* RogueLikeDungeon::instance = nullptr;

//�R���X�g���N�^
RogueLikeDungeon::RogueLikeDungeon()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

//�f�X�g���N�^
RogueLikeDungeon::~RogueLikeDungeon()
{
	//	���
	map_role.clear();
	map_role.shrink_to_fit();
}

void RogueLikeDungeon::InitializeMapSize()
{
	//�Đݒ�
	map_role.resize(MapSize_Y, std::vector<RogueLikeMap>(MapSize_X, 0));
}

//�}�b�v���̍X�V
void RogueLikeDungeon::UpdateMapRole()
{
	//��x�㏑��
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//�ǂȂ疳��(����A�ǂ𖳎�����G�̈ړ����ǉ������Ȃ�ύX����)
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				continue;
			}
			//�K�i�Ȃ疳������
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Exit))
			{
				continue;
			}
			//\�����ɃA�C�e����㩂���������4,5
			//���ɏ�������

			if (map_role[y][x].is_room == false)//���̑������ʘH
			{
				map_role[y][x].map_data = static_cast<size_t>(Attribute::Road);
			}
			else if (map_role[y][x].is_room == true && map_role[y][x].map_data != static_cast<size_t>(Attribute::Exit))//���̑������������͊K�i�ł͂Ȃ�
			{
				map_role[y][x].map_data = static_cast<size_t>(Attribute::Room);
			}
		}
	}

	//�X�V
	const CharacterManager& character_manager = CharacterManager::Instance();
	for (const auto& enemy : character_manager.GetCharacters())
	{
		if (enemy->GetId() < static_cast<int>(Identity::Enemy))//ID���G�̒l�łȂ��Ȃ�ȉ��Ȃ�
		{
			continue;
		}
		const DirectX::XMFLOAT2 enemy_pos = DirectX::XMFLOAT2(enemy->GetPosition().x / CellSize, enemy->GetPosition().z / CellSize);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

	//�X�V��̓G�̃f�[�^�̏�������
		map_role[static_cast<size_t>(enemy_pos.y)][static_cast<size_t>(enemy_pos.x)].map_data = static_cast<int>(Attribute::Enemy);
	}

	const DirectX::XMFLOAT3 pl_pos = character_manager.GetCharacterFromId(static_cast<int>(Identity::Player))->GetPosition();
	const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(pl_pos.x / CellSize, pl_pos.z / CellSize);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

	//�X�V��̃v���C���[�̃f�[�^�̏�������
	map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x)].map_data = static_cast<int>(Attribute::Player);
}

void RogueLikeDungeon::MakeDungeon()
{
	//�Ȃ���
	srand(static_cast<unsigned int>(time(nullptr)));

	//	���
	ClearMap();

	//�T�C�Y�̍Đݒ�
	InitializeMapSize();

	//�}�b�v�̐���
	MakeMap();

	for (int i = 0; i < ObjectMax; i++)
	{
		//�����_����Object�̈ʒu��ݒ肷��
		SetObjectPos(i);
	}
}

void RogueLikeDungeon::ClearMap()
{
	//	���
	map_role.clear();
	map_role.shrink_to_fit();
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
			ImGui::Text("Exit:2");
			ImGui::Text("Room:3");
			ImGui::Text("Road:4");
			ImGui::Text("Player:5");
			ImGui::Text("Enemy:6");
			/*Attribute
			Wall = 0,// 0:��
			Floor,    // 1:��
			Exit,		// 2:�K�i
			Room,	// 3:����
			Road,	// 4:�ʘH
			Player,   // 5:�v���C���[
			Enemy,  // 6:�G
			// 7 : �A�C�e��
			// 8 : �
			// 9 : �A�C�e���̏�̃v���C���[
			// 10 : �A�C�e���̏�̓G
			// 11 : �n�`�̏�̃v���C���[
			// 12 :  �n�`�̏�̓G
			EndAttribute
			*/
		}
	}
	ImGui::End();
}

void RogueLikeDungeon::SetObjectPos(const int id)
{
	//Mob��ݒu����
	const int random_room_id = static_cast<int>(dungeon_map_role.map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room_count)))]); //�}�b�v��̕����������_���Ɏw�肷��

	const int random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][0] - dungeon_map_role.map_room[random_room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
	int position_y = static_cast<int>(dungeon_map_role.map_room[random_room_id][2]) + random_pos_y; //�}�b�v��̕����̃����_����Y���W

	const int random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][1] - dungeon_map_role.map_room[random_room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
	int position_x = static_cast<int>(dungeon_map_role.map_room[random_room_id][3]) + random_pos_x; //�}�b�v��̕����̃����_����X���W

	//�P�̃I�u�W�F�N�g�̈ʒu�ݒ�
	if (id == 0)	//id��0�Ԗڂ̓v���C���[
	{
		//	�v���C���[�̈ʒu��ۑ�
		player_pos = { position_x,position_y };
		map_room_player = random_room_id; //�����Ƀv���C���[������
	}
	else if (id == 1)	//id��1�Ԗڂ͊K�i
	{
		//	�K�i�̈ʒu��ۑ�
		stairs_pos = { position_x,position_y };
	}
	else
	{
		//while (
		//	Math::Comparison(player_pos.y, static_cast<float>(position_y)) && Math::Comparison(player_pos.x, static_cast<float>(position_x)) //�G���v���C���[�Əd�Ȃ��Ă���
		//																												||																													//��������
		//	Math::Comparison(stairs_pos.y, static_cast<float>(position_y)) &&Math::Comparison(stairs_pos.x, static_cast<float>(position_x))	//�G���K�i�Əd�Ȃ��Ă���
		//	)
		//{//�v���C���[�Əd�Ȃ��Ă��Ȃ������炱�̃��[�v�𔲂���
		//	//Mob��ݒu����
		//	random_room_id = static_cast<int>(dungeon_map_role.map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room_count)))]); //�}�b�v��̕����������_���Ɏw�肷��
		//	random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][0] - dungeon_map_role.map_room[random_room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
		//	position_y = static_cast<int>(dungeon_map_role.map_room[random_room_id][2]) + random_pos_y; //�}�b�v��̕����̃����_����Y���W
		//	random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][1] - dungeon_map_role.map_room[random_room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
		//	position_x = static_cast<int>(dungeon_map_role.map_room[random_room_id][3]) + random_pos_x; //�}�b�v��̕����̃����_����X���W
		//}
		//�G�̈ʒu
		//if (!Math::Comparison(player_pos.y, static_cast<float>(position_y)) &&
		//	!Math::Comparison(player_pos.x, static_cast<float>(position_x))&&
		//	!Math::Comparison(stairs_pos.y, static_cast<float>(position_y)) &&
		//	!Math::Comparison(stairs_pos.x, static_cast<float>(position_x)))
		//{
		//map_role[position_y][position_x].map_data = static_cast<size_t>(Attribute::Enemy);
		//}
		map_role[position_y][position_x].map_data = static_cast<size_t>(Attribute::Enemy);
	}

	if (id == ObjectMax - ONE)//id���ő吔�ɒB������
	{//�P�̃I�u�W�F�N�g�̔z�u
		while (Math::Comparison(static_cast<float>(player_pos.y), static_cast<float>(stairs_pos.y)) && Math::Comparison(static_cast<float>(player_pos.x), static_cast<float>(stairs_pos.x))) //�K�i�ƃv���C���[�Əd�Ȃ��Ă���
		{
			const int room_id = static_cast<int>(dungeon_map_role.map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room_count)))]); //�}�b�v��̕����������_���Ɏw�肷��

			 int pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_id][0] - dungeon_map_role.map_room[room_id][2]))); //�}�b�v��Y���W�̒����̒����烉���_���Ɏw��
			pos_y = static_cast<int>(dungeon_map_role.map_room[room_id][2]) + pos_y; //�}�b�v��̕����̃����_����Y���W

			 int pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_id][1] - dungeon_map_role.map_room[room_id][3]))); //�}�b�v��X���W�̒����̒����烉���_���Ɏw��
			pos_x = static_cast<int>(dungeon_map_role.map_room[room_id][3]) + pos_x; //�}�b�v��̕����̃����_����X���W
			player_pos = DirectX::XMINT2(pos_x,pos_y);
		}
		map_role[player_pos.y][player_pos.x].map_data = static_cast<size_t>(Attribute::Player);
		map_role[stairs_pos.y][stairs_pos.x].map_data = static_cast<size_t>(Attribute::Exit);
	}


}

void RogueLikeDungeon::MakeMap()
{
	if (map_role.empty() || map_role.front().empty())
	{

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
	/*�ʘH�͂Q�����Ԃׂ̍��ʘH�̂��Ƃ��w���B
	�ʘH����邽�߂ɂQ���������ꂼ��O(Before)�ƌ�(After)�ŕ�����B
	�S�Ă̕������`�F�b�N���A�O��̕������q���ʘH�����B
	�܂��A�O�̒ʘH�����A���Ɍ�̒ʘH�����B
	�Ō�ɑO�ƌ�̒ʘH���q����B
	*/
	//�O�͒ʘH�����΂�����
	//��͒ʘH���q���镔��

	size_t room_after{};
	for (size_t room_before = 0; room_before < dungeon_map_role.map_division_count; room_before++)
	{
		room_after = dungeon_map_role.map_road[room_before][0];/*�q�����̕���ID*/

		bool is_once = false;//for�����A��x�����s����Ƃ̂��߂̃t���O

		//Y���W�̒ʘH
		switch (dungeon_map_role.map_road[room_before][1]/*0:Y���W , 1:X���W*/)
		{
		case static_cast<size_t>(Road::Axis_Y):

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

		case static_cast<size_t>(Road::Axis_X):

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

	}

	//�ʘH�̓�����������i�[����
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (map_role[y][x].is_road_entrance == true)
			{
				DirectX::XMFLOAT2 entrance_array = { static_cast<float>(y),static_cast<float>(x) };
				roads_entrance.emplace_back(entrance_array);
			}
		}
	}


}
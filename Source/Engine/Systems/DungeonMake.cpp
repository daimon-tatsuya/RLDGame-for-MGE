
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
	//更新前のプレイヤーのデータの書き換え
	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//属性がプレイヤーなら
			if (map_role[y][x].map_data == 2)
			{
				//足元にアイテムか罠があったら4,5
				//床に書き換え
				map_role[y][x].map_data = 1;
			}
		}
	}

	//更新
	CharacterManager& character_manager = CharacterManager::Instance();

	Character* player = character_manager.GetPlayer();

	DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(player->GetPosition().x / Cell_Size, player->GetPosition().z / Cell_Size);//データ上の値にするためCell_Sizeで割る

	//更新後のプレイヤーのデータの書き換え
	map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x)].map_data = 2;

}

void RogueLikeDungeon::UpdateMapRoleEnemis()
{
	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//属性が敵なら
			if (map_role[y][x].map_data == 3)
			{
				//足元にアイテムか罠があったら4,5
				//床に書き換え
				map_role[y][x].map_data = 1;
			}
		}
	}
	//更新
	CharacterManager& character_manager = CharacterManager::Instance();
	for (auto& enemy : character_manager.GetEnemis())
	{
		DirectX::XMFLOAT2 enemy_pos = DirectX::XMFLOAT2(enemy->GetPosition().x / Cell_Size, enemy->GetPosition().z / Cell_Size);//データ上の値にするためCell_Sizeで割る

	//更新後のプレイヤーのデータの書き換え
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
	//Mobを設置する
	int random_room_id = dungeon_map_role->map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room_count)))]; //マップ上の部屋をランダムに指定する

	int random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][0] - dungeon_map_role->map_room[random_room_id][2]))); //マップのY座標の長さの中からランダムに指定
	int position_y = static_cast<int>(dungeon_map_role->map_room[random_room_id][2]) + random_pos_y; //マップ上の部屋のランダムなY座標

	int random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][1] - dungeon_map_role->map_room[random_room_id][3]))); //マップのX座標の長さの中からランダムに指定
	int position_x = static_cast<int>(dungeon_map_role->map_room[random_room_id][3]) + random_pos_x; //マップ上の部屋のランダムなX座標

	//idの0番目はplayer
	if (id == 0)
	{
		//	プレイヤーの位置
		map_role[position_y][position_x].map_data = 2;
		mobs[id].position = { static_cast<float>(position_x),static_cast<float>(position_y) };
		dungeon_map_role->map_room_player[random_room_id] = 1; //部屋にプレイヤーがいる(侵入)
	}
	else//id!=0
	{
		while (((mobs[0].position.y == position_y) && (mobs[0].position.x == position_x)))//プレイヤーと重なっているなら
		{
			//Mobを設置する
			random_room_id = dungeon_map_role->map_room_id[static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room_count)))]; //マップ上の部屋をランダムに指定する

			random_pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][0] - dungeon_map_role->map_room[random_room_id][2]))); //マップのY座標の長さの中からランダムに指定
			position_y = static_cast<int>(dungeon_map_role->map_room[random_room_id][2]) + random_pos_y; //マップ上の部屋のランダムなY座標

			random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[random_room_id][1] - dungeon_map_role->map_room[random_room_id][3]))); //マップのX座標の長さの中からランダムに指定
			position_x = static_cast<int>(dungeon_map_role->map_room[random_room_id][3]) + random_pos_x; //マップ上の部屋のランダムなX座標
		}
		//敵の位置
		map_role[position_y][position_x].map_data = 3;
		mobs[id].position = { static_cast<float>(position_x),static_cast<float>(position_y) };
	}
	//mobs[id].id = 1;

	////他のmobと重なっていたら消す
	//for (int i = 0; i < Mob_Max; i++)
	//{
	//	//床でも壁でもない
	//	if (map_role[position_y][position_x].map_data < 1)
	//	{
	//		mobs[id].id = 0;
	//		break;
	//	}
	//}
	//エラーチェック
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
	//マップの区分け数 (部屋の個数) 0~nまでの部屋ID
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

	dungeon_map_role->map_division[0][0] = (map_role.size() - 1); //マップの区分け初期サイズY終点 (マップの大きさY軸)
	dungeon_map_role->map_division[0][1] = (map_role.front().size() - 1); //マップの区分け初期サイズX終点 (マップの大きさX軸)
	dungeon_map_role->map_division[0][2] = 1; //マップの区分け初期サイズX始点 (マップの大きさY軸)
	dungeon_map_role->map_division[0][3] = 1; //マップの区分け初期サイズY始点 (マップの大きさX軸)

	dungeon_map_role->map_road[0][0] = 255;
	dungeon_map_role->map_road[0][1] = 255;

	/*マップを区分けしていく処理(区域を分割する処理)*/
	size_t division_after;
	int count; //(0:Y, 1:X) Y軸で分けるかX軸で分けるか決める

	for (size_t i = 1; i < dungeon_map_role->map_division_count; i++)
	{
		//今まで作った区分けをランダムに指定(指定した区域をさらに区分けする)
		division_after = static_cast<size_t>(Math::RandomInt(static_cast<int>(i)));

		//指定した区域のYとXの長さによって、分割する向きを決める(長いほうを分割する)
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
		//count軸の設定

		//0.軸の右端(iR)の座標(divAfter*R/3~2divAfter*R/3)
		dungeon_map_role->map_division[i][count] = dungeon_map_role->map_division[division_after][count + 2] +
			(dungeon_map_role->map_division[division_after][count] - dungeon_map_role->map_division[division_after][count + 2]) / 3 +
			static_cast<size_t>(Math::RandomInt(((dungeon_map_role->map_division[division_after][count] - dungeon_map_role->map_division[division_after][count + 2]) / 3)));

		dungeon_map_role->map_division[i][count + 2] = dungeon_map_role->map_division[division_after][count + 2]; //0.軸の左端(iL)の座標(divAfterL)
		dungeon_map_role->map_division[division_after][count + 2] = dungeon_map_role->map_division[i][count]; //divAfter軸の左端(divAfterL)の座標(iR)

		//countとは逆の軸の設定
		dungeon_map_role->map_division[i][abs(count - 1)] = dungeon_map_role->map_division[division_after][abs(count - 1)]; //軸の右端(iR)の座標(divAfterR)
		dungeon_map_role->map_division[i][abs(count - 1) + 2] = dungeon_map_role->map_division[division_after][abs(count - 1) + 2]; //軸の左端(iL)の座標(divAfterL)
	}

	/*部屋を生成する処理*/
	for (size_t i = 0; i < dungeon_map_role->map_division_count; i++)//区分け
	{
		dungeon_map_role->map_room_player[i] = 0;//プレイヤー侵入初期化
		dungeon_map_role->map_room[i][2] = dungeon_map_role->map_division[i][2]; //区分けY始点をマップY始点へ代入
		dungeon_map_role->map_room[i][3] = dungeon_map_role->map_division[i][3]; //区分けX始点をマップX始点へ代入

		//Y座標の部屋の長さを指定
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
		//Todo マジックナンバー
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

	/*通路を生成する処理*/
	/*通路は２部屋間の細い道のことを指す。
	通路を作るために２部屋をそれぞれ前(Before)と後(After)で分ける。
	for文で全ての部屋をチェックし、前後の部屋を繋ぐ通路を作る。
	まず、前の通路を作り、次に後の通路を作る。
	最後に前と後の通路を繋げる。
	*/
	size_t room_after;
	for (size_t room_before = 0; room_before < dungeon_map_role->map_division_count; room_before++)
	{
		room_after = dungeon_map_role->map_road[room_before][0];

		//Y座標の通路
		switch (dungeon_map_role->map_road[room_before][1])
		{
			case static_cast<size_t>(Road::Axis_Y) :
				//前側の通路の位置
				dungeon_map_role->map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_before][1] - dungeon_map_role->map_room[room_before][3] - 2)));
				//後側の通路の位置
				dungeon_map_role->map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_after][1] - dungeon_map_role->map_room[room_after][3] - 2)));
				//前の通路
				for (size_t j = dungeon_map_role->map_room[room_before][0]; j < dungeon_map_role->map_division[room_before][0]; j++)
				{
					map_role[j][dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][3]].map_data = 1; //通路をマップチップに線画
				}
				//後の通路
				for (size_t j = dungeon_map_role->map_division[room_after][2]; j < dungeon_map_role->map_room[room_after][2]; j++)
				{
					map_role[j][dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][3]].map_data = 1; //通路をマップチップに線画
				}
				//通路をつなぐ
				for (size_t j = dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][3]; j <= dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][3]; j++)
				{
					map_role[dungeon_map_role->map_division[room_before][0]][j].map_data = 1; //通路をマップチップに線画 2から5(上から下)
				}
				for (size_t j = dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][3]; j <= dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][3]; j++)
				{
					map_role[dungeon_map_role->map_division[room_before][0]][j].map_data = 1; //通路をマップチップに線画 5から2(下から上)
				}
				break;

				case static_cast<size_t>(Road::Axis_X) :
					//前側の通路の位置
					dungeon_map_role->map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_before][0] - dungeon_map_role->map_room[room_before][2] - ONE)));
					//後側の通路の位置
					dungeon_map_role->map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role->map_room[room_after][0] - dungeon_map_role->map_room[room_after][2] - ONE)));
					//前の通路
					for (size_t j = dungeon_map_role->map_room[room_before][1]; j < dungeon_map_role->map_division[room_before][1]; j++)
					{
						map_role[dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][2]][j].map_data = 1; //通路をマップチップに線画
					}
					//後の通路
					for (size_t j = dungeon_map_role->map_division[room_after][3]; j < dungeon_map_role->map_room[room_after][3]; j++)
					{
						map_role[dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][2]][j].map_data = 1; //通路をマップチップに線画
					}
					//通路をつなぐ
					for (size_t j = dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][2]; j <= dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][2]; j++)
					{
						map_role[j][dungeon_map_role->map_division[room_before][1]].map_data = 1; //通路をマップチップに線画
					}
					for (size_t j = dungeon_map_role->map_road[room_before][3] + dungeon_map_role->map_room[room_after][2]; j <= dungeon_map_role->map_road[room_before][2] + dungeon_map_role->map_room[room_before][2]; j++)
					{
						map_role[j][dungeon_map_role->map_division[room_before][1]].map_data = 1; //通路をマップチップに線画
					}
					break;
		}
	}
	//エラーチェック
	if (!dungeon_map_role->map_division_count)
	{
		return false;
	}

	return true;
}
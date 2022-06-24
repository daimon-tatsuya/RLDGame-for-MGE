//**********************************************************
//
//		RogueLikeDungeonクラス
//
//**********************************************************

//! xyが最大最小値を超えないようにxyが０または最大値の時、変更されない属性を設定する

#include <ctime>

#include "Engine/AI/DungeonMake.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine//Systems/Character.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/Math.h"


//コンストラクタ
RogueLikeDungeon::RogueLikeDungeon()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

//デストラクタ
RogueLikeDungeon::~RogueLikeDungeon()
{
	//	解放
	ClearMap();
}

void RogueLikeDungeon::InitializeMapSize()
{
	//再設定
	map_role.resize(MapSize_Y, std::vector<RogueLikeMap>(MapSize_X, 0));
}

//マップ情報の更新
void RogueLikeDungeon::UpdateMapRole()
{
	//一度上書き
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//壁なら無視(今後、壁を無視する敵の移動が追加されるなら変更する)
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				continue;
			}
			//階段なら無視する
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Exit))
			{
				continue;
			}

			//床に書き換え
			if (map_role[y][x].is_room == false)//床の属性が通路
			{
				map_role[y][x].map_data = static_cast<size_t>(Attribute::Road);
			}
			else if (map_role[y][x].is_room == true && map_role[y][x].map_data != static_cast<size_t>(Attribute::Exit))//床の属性が部屋又は階段ではない
			{
				map_role[y][x].map_data = static_cast<size_t>(Attribute::Room);
			}
		}
	}

	//更新
	const CharacterManager& character_manager = CharacterManager::Instance();
	for (const auto& enemy : character_manager.GetCharacters())
	{
		if (enemy->GetId() < static_cast<int>(Identity::Enemy))//IDが敵の値でないなら以下なら
		{
			continue;
		}
		const DirectX::XMFLOAT2 enemy_pos = DirectX::XMFLOAT2(enemy->GetPosition().x / CellSize, enemy->GetPosition().z / CellSize);//データ上の値にするためCell_Sizeで割る

	//更新後の敵のデータの書き換え
		map_role[static_cast<size_t>(enemy_pos.y)][static_cast<size_t>(enemy_pos.x)].map_data = static_cast<int>(Attribute::Enemy);
	}

	const DirectX::XMFLOAT3 pl_pos = character_manager.GetCharacterFromId(static_cast<int>(Identity::Player))->GetPosition();
	const DirectX::XMINT2 player_pos = DirectX::XMINT2(static_cast<int>(pl_pos.x / CellSize), static_cast<int>(pl_pos.z / CellSize)); //データ上の値にするためCell_Sizeで割る

	//更新後のプレイヤーのデータの書き換え
	map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x)].map_data = static_cast<int>(Attribute::Player);

	//更新後の階段のデータの書き換え
	map_role[static_cast<size_t>(stairs_pos.y)][static_cast<size_t>(stairs_pos.x)].map_data = static_cast<int>(Attribute::Exit);

	map_room_player = GetLocateRoomNunber(player_pos);
}

//
void RogueLikeDungeon::MakeDungeon()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	//	解放
	ClearMap();

	//サイズの再設定
	InitializeMapSize();

	//マップの生成
	MakeMap();

	for (int i = 0; i < ObjectMax; i++)
	{
		//ランダムにObjectの位置を設定する
		SetObjectPos(i);
	}
}


//	マップ情報のの解放
void RogueLikeDungeon::ClearMap()
{
	//	コンテナの解放

	map_role.clear();
	map_role.shrink_to_fit();

	roads.clear();
	roads.shrink_to_fit();

	rooms_center.clear();
	rooms_center.shrink_to_fit();
}


void RogueLikeDungeon::DrawDebugGUI() const
{
	ImGui::SetNextWindowPos(ImVec2(0, 350), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 350), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("MapInformation ", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Text("MapSize:%d \nMapSize_X : %d MapSize_Y : %d", MapSize, MapSize_X, MapSize_Y);
		ImGui::Text("MapRoomCount : %d", static_cast<int>(dungeon_map_role.map_room_count));//部屋の数
		ImGui::Text("MapDivisionCount : %d", static_cast<int>(dungeon_map_role.map_division_count));//部屋を作るために分割した数(部屋の数と一致しない場合通路がねじれている)
		ImGui::Text("PlayerRoomNumber : %d", static_cast<int>(map_room_player));//プレイヤーのいる部屋番号


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
			Wall = 0,// 0:壁
			Floor,    // 1:床
			Exit,		// 2:階段
			Room,	// 3:部屋
			Road,	// 4:通路
			Player,   // 5:プレイヤー
			Enemy,  // 6:敵
			// 7 : アイテム
			// 8 : 罠
			// 9 : アイテムの上のプレイヤー
			// 10 : アイテムの上の敵
			// 11 : 地形の上のプレイヤー
			// 12 :  地形の上の敵
			EndAttribute
			*/
		}
		if (ImGui::CollapsingHeader("Roads", ImGuiTreeNodeFlags_None))
		{
			ImGui::Text("RoadsNum : %zu ", roads.size());
			for (auto& road : roads)
			{
				ImGui::Text("road : x: %d  y : %d", road.x, road.y);
			}
		}
		if (ImGui::CollapsingHeader("RoomCenter", ImGuiTreeNodeFlags_None))
		{
			ImGui::Text("RoomNum : %zu ", rooms_center.size());
			for (auto& room : rooms_center)
			{
				ImGui::Text("room_center : x : %d  y : %d", room.x, room.y);
			}
		}
	}
	ImGui::End();
}

int RogueLikeDungeon::GetLocateRoomNunber(DirectX::XMINT2 object_pos)
{
	//if (dungeon_map_role.map_room_count > 0)//初期化されていないなら判別しない
	//{
	for (int i = 0; i <= static_cast<int>(dungeon_map_role.map_room_count); i++)
	{
		const int end_y = static_cast<int>(dungeon_map_role.map_room[i][0]);//部屋のY座標の終点
		const int end_x = static_cast<int>(dungeon_map_role.map_room[i][1]);//部屋のX座標の終点
		const int start_y = static_cast<int>(dungeon_map_role.map_room[i][2]);//部屋のY座標の始点
		const int start_x = static_cast<int>(dungeon_map_role.map_room[i][3]);//部屋のX座標の始点
		if (
			(object_pos.y <= end_y &&  //オブジェクトのy位置が終点より小さい且つ
				object_pos.y >= start_y) && //オブジェクトのy位置が始点より大きい且つ
			(object_pos.x <= end_x &&   //オブジェクトのx位置が終点より小さい且つ
				object_pos.x >= start_x)   //オブジェクトのx位置が始点より大きい且つ
			)
		{
			return i;
		}
		//}
	}
	//部屋にはいない
	return -1;
}

// マップ情報をもとにマップ上の部屋にobjectの位置を設定する
void RogueLikeDungeon::SetObjectPos(const int id)
{
	//オブジェクトを設置する
	const int random_room_id = static_cast<int>
		(dungeon_map_role.map_room_id[static_cast<size_t>
			(Math::RandomInt(static_cast<int>
				(dungeon_map_role.map_room_count)))]); //マップ上の部屋をランダムに指定する
		//y位置
	const int random_pos_y = (Math::RandomInt(static_cast<int>
		(dungeon_map_role.map_room[random_room_id][0] -
			dungeon_map_role.map_room[random_room_id][2]))); //マップのY座標の長さの中からランダムに指定
	int position_y = static_cast<int>(dungeon_map_role.map_room[random_room_id][2]) + random_pos_y; //マップ上の部屋のランダムなY座標
	//x位置
	const int random_pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[random_room_id][1] - dungeon_map_role.map_room[random_room_id][3]))); //マップのX座標の長さの中からランダムに指定
	int position_x = static_cast<int>(dungeon_map_role.map_room[random_room_id][3]) + random_pos_x; //マップ上の部屋のランダムなX座標

	//唯一のオブジェクトの位置設定
	if (id == 0)	//idの0番目はプレイヤー
	{
		//	プレイヤーの位置を保存
		player_pos = { position_x,position_y };
		map_room_player = random_room_id; //部屋にプレイヤーがいる
	}
	else if (id == 1)	//idの1番目は階段
	{
		//	階段の位置を保存
		stairs_pos = { position_x,position_y };
	}
	else//マップ情報に敵の属性を設定
	{
		map_role[position_y][position_x].map_data = static_cast<size_t>(Attribute::Enemy);
	}

	//唯一のオブジェクトが上書きされるのを防止するために
	if (id == ObjectMax - ONE)//idが最大数に達したら
	{//唯一のオブジェクトの配置
		while (Math::Comparison(static_cast<float>(player_pos.y), static_cast<float>(stairs_pos.y))
			&& Math::Comparison(static_cast<float>(player_pos.x), static_cast<float>(stairs_pos.x))) //階段とプレイヤーと重なっている
		{
			const int room_id = static_cast<int>(dungeon_map_role.map_room_id[static_cast<size_t>
				(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room_count)))]); //マップ上の部屋をランダムに指定する

			int pos_y = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_id][0]
				- dungeon_map_role.map_room[room_id][2]))); //マップのY座標の長さの中からランダムに指定
			pos_y = static_cast<int>(dungeon_map_role.map_room[room_id][2]) + pos_y; //マップ上の部屋のランダムなY座標

			int pos_x = (Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_id][1] - dungeon_map_role.map_room[room_id][3]))); //マップのX座標の長さの中からランダムに指定
			pos_x = static_cast<int>(dungeon_map_role.map_room[room_id][3]) + pos_x; //マップ上の部屋のランダムなX座標
			player_pos = DirectX::XMINT2(pos_x, pos_y);
		}
		//	マップ情報に唯一のオブジェクトの属性を設定
		map_role[player_pos.y][player_pos.x].map_data = static_cast<size_t>(Attribute::Player);
		map_role[stairs_pos.y][stairs_pos.x].map_data = static_cast<size_t>(Attribute::Exit);
	}


}

bool RogueLikeDungeon::MakeMap()
{
	//コンテナが空でだったら
	//つまりサイズを確保出来ていない
	if (map_role.empty() || map_role.front().empty())
	{
		return false;
	}

	//マップを壁で埋める初期化する
	for (auto& i : map_role)
	{
		for (auto& j : i)
		{
			j.map_data = static_cast<size_t>(Attribute::Wall);
			j.is_room = false;
		}
	}
	//マップの区分け数を設定
	dungeon_map_role.map_division_count = dungeon_map_role.division_count_min +
		static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.division_count_rand + ONE)));

	//	区分けの数が最大数を超えたら
	if (dungeon_map_role.map_division_count > dungeon_map_role.division_count_max)
	{
		return false;
	}

	//部屋の初期化
	for (int i = 0; i < dungeon_map_role.map_division_count; i++)
	{
		dungeon_map_role.map_room_area[i] = 0;
	}

	dungeon_map_role.map_division[0][0] = (map_role.size() - 1); //マップの区分け初期サイズY終点 (マップの大きさY軸)
	dungeon_map_role.map_division[0][1] = (map_role.front().size() - 1); //マップの区分け初期サイズX終点 (マップの大きさX軸)
	dungeon_map_role.map_division[0][2] = 1; //マップの区分け初期サイズX始点 (マップの大きさY軸)
	dungeon_map_role.map_division[0][3] = 1; //マップの区分け初期サイズY始点 (マップの大きさX軸)

	dungeon_map_role.map_road[0][0] = 255;
	dungeon_map_role.map_road[0][1] = 255;

	/*マップを区分けしていく処理(区域に分割する処理)*/

	size_t division_after{};
	int count{}; //(0:Y, 1:X) Y軸で分けるかX軸で分けるか決める

	for (size_t i = 1; i < dungeon_map_role.map_division_count; i++)
	{
		//今まで作った区分けをランダムに指定(指定した区域をさらに区分けする)
		division_after = static_cast<size_t>(Math::RandomInt(static_cast<int>(i)));

		//指定した区域のYとXの長さによって、分割する向きを決める(長いほうを分割する)
		if (dungeon_map_role.map_division[division_after][0] - dungeon_map_role.map_division[division_after][2] >
			dungeon_map_role.map_division[division_after][1] - dungeon_map_role.map_division[division_after][3])
		{
			count = static_cast<int>(Road::Axis_Y);
		}
		else
		{
			count = static_cast<int>(Road::Axis_X);
		}

		//分割線を引く処理
		if (dungeon_map_role.map_division[division_after][count] - dungeon_map_role.map_division[division_after][count + 2] <
			dungeon_map_role.division_count_rand * 2 + RoomMax)
		{
			size_t k = 0;
			for (size_t j = 1; j < dungeon_map_role.map_division_count; j++)
			{
				//マップ分割Y　 Y終点 - Y始点
				if (dungeon_map_role.map_division[j][0] - dungeon_map_role.map_division[j][2] > k)
				{
					k = dungeon_map_role.map_division[j][0] - dungeon_map_role.map_division[j][2];
					division_after = j;
					count = static_cast<int>(Road::Axis_Y);
				}
				//マップ分割X　 X終点 - X始点
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
		//count軸の設定

		//0.軸の右端(iR)の座標(divAfter*R/3~2divAfter*R/3)
		dungeon_map_role.map_division[i][count] = dungeon_map_role.map_division[division_after][count + 2] +
			(dungeon_map_role.map_division[division_after][count] - dungeon_map_role.map_division[division_after][count + 2]) / 3 +
			static_cast<size_t>
			(Math::RandomInt
			(
				((static_cast<int>(dungeon_map_role.map_division[division_after][count]) - static_cast<int>(dungeon_map_role.map_division[division_after][count + 2])) / 3)
			)
				);

		dungeon_map_role.map_division[i][count + 2] = dungeon_map_role.map_division[division_after][count + 2]; //0.軸の左端(iL)の座標(divAfterL)
		dungeon_map_role.map_division[division_after][count + 2] = dungeon_map_role.map_division[i][count]; //divAfter軸の左端(divAfterL)の座標(iR)

		//countとは逆の軸の設定
		dungeon_map_role.map_division[i][abs(count - 1)] = dungeon_map_role.map_division[division_after][abs(count - 1)]; //軸の右端(iR)の座標(divAfterR)
		dungeon_map_role.map_division[i][abs(count - 1) + 2] = dungeon_map_role.map_division[division_after][abs(count - 1) + 2]; //軸の左端(iL)の座標(divAfterL)
	}

	/* 部屋を生成する処理 */
	for (size_t i = 0; i < dungeon_map_role.map_division_count; i++)//区分け
	{
		map_room_player = 0;//プレイヤー侵入初期化
		dungeon_map_role.map_room[i][2] = dungeon_map_role.map_division[i][2]; //区分けY始点をマップY始点へ代入
		dungeon_map_role.map_room[i][3] = dungeon_map_role.map_division[i][3]; //区分けX始点をマップX始点へ代入

		/* Y座標の部屋の長さを指定 */

		//部屋iの	Y軸の終点 ＝ 区域iのY軸の始点+区分けのランダム値+1~任意の値(dungeon_map_role.room_length_randY)までのランダム値
		dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][2] + dungeon_map_role.room_length_minY
			+ static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.room_length_randY + ONE)));

		/* 部屋の大きさを区域内に収める */

		//区域iのY軸の長さが部屋iのY軸の長さ+5より小さいなら
		if (dungeon_map_role.map_division[i][0] - dungeon_map_role.map_division[i][2] < dungeon_map_role.map_room[i][0] - dungeon_map_role.map_room[i][2] + 5)
		{
			//部屋iのY軸の終点を区域iのY軸終点-5にして範囲に収める
			dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][0] - 5;

			//	//
			//	if (dungeon_map_role.map_division[i][0] - dungeon_map_role.map_division[i][2] < dungeon_map_role.map_room[i][0] - dungeon_map_role.map_room[i][2] + 5)
			//	{
			//		dungeon_map_role.map_room[i][0] = dungeon_map_role.map_division[i][2] +1;
			//	}
		}
		/* X座標の部屋の長さを指定*/

		//部屋iのX軸の終点 ＝ 区域iのX軸の始点+区分けのランダム値+1~任意の値(dungeon_map_role.room_length_randX)までのランダム値
		dungeon_map_role.map_room[i][1] = dungeon_map_role.map_division[i][3] + dungeon_map_role.room_length_minX
			+ static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.room_length_randX + ONE)));

		//区域iのX軸の長さが部屋iのX軸の長さ+5より小さいなら
		if (dungeon_map_role.map_division[i][1] - dungeon_map_role.map_division[i][3] < dungeon_map_role.map_room[i][1] - dungeon_map_role.map_room[i][3] + 5)
		{
			//部屋iのX軸の終点を区域iのX軸終点-5にして範囲に収める
			dungeon_map_role.map_room[i][1] = dungeon_map_role.map_division[i][1] - 5;

		}
		//部屋のx,yの長さが1以下なら
		if (dungeon_map_role.map_room[i][0] - dungeon_map_role.map_division[i][2] <= 1 ||
			dungeon_map_role.map_room[i][1] - dungeon_map_role.map_division[i][3] <= 1)
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

		//部屋の床の配置
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

	/*通路を生成する処理*/
	/*通路は２部屋間の細い通路のことを指す。
	通路を作るために２部屋をそれぞれ前(Before)と後(After)で分ける。
	全ての部屋をチェックし、前後の部屋を繋ぐ通路を作る。
	まず、前の通路を作り、次に後の通路を作る。
	最後に前と後の通路を繋げる。
	*/
	//前は通路を延ばす部屋
	//後は通路が繋がる部屋

	size_t room_after{};
	for (size_t room_before = 0; room_before < dungeon_map_role.map_division_count; room_before++)
	{
		room_after = dungeon_map_role.map_road[room_before][0];/*繋がる先の部屋ID*/

		//Y座標の通路
		switch (dungeon_map_role.map_road[room_before][1]/*0:Y座標 , 1:X座標*/)
		{
		case static_cast<size_t>(Road::Axis_Y):

			//前側の通路の位置
			dungeon_map_role.map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_before][1] - dungeon_map_role.map_room[room_before][3] - 2)));
			//後側の通路の位置
			dungeon_map_role.map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_after][1] - dungeon_map_role.map_room[room_after][3] - 2)));

			//前の通路を分割線まで伸ばす
			for (size_t j = dungeon_map_role.map_room[room_before][0]; j < dungeon_map_role.map_division[room_before][0]; j++)
			{
				map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定
				map_role[j][dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]].is_room = false;//このデータは通路
			}


			//後の通路を分割線まで伸ばす
			for (size_t j = dungeon_map_role.map_division[room_after][2]; j < dungeon_map_role.map_room[room_after][2]; j++)
			{
				map_role[j][dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定
				map_role[j][dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]].is_room = false;//このデータは通路
			}

			//通路をつなぐ(分割線上に伸ばす)
			for (size_t j = dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]; j <= dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]; j++)
			{
				map_role[dungeon_map_role.map_division[room_before][0]][j].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定 2から5(上から下)
				map_role[dungeon_map_role.map_division[room_before][0]][j].is_room = false;//このデータは通路
			}

			for (size_t j = dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][3]; j <= dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][3]; j++)
			{
				map_role[dungeon_map_role.map_division[room_before][0]][j].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定 5から2(下から上)
				map_role[dungeon_map_role.map_division[room_before][0]][j].is_room = false;//このデータは通路
			}
			break;

		case static_cast<size_t>(Road::Axis_X):

			//前側の通路の位置
			dungeon_map_role.map_road[room_before][2] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_before][0] - dungeon_map_role.map_room[room_before][2] - ONE)));

			//後側の通路の位置
			dungeon_map_role.map_road[room_before][3] = static_cast<size_t>(Math::RandomInt(static_cast<int>(dungeon_map_role.map_room[room_after][0] - dungeon_map_role.map_room[room_after][2] - ONE)));

			//前の通路を分割線まで伸ばす
			for (size_t j = dungeon_map_role.map_room[room_before][1]; j < dungeon_map_role.map_division[room_before][1]; j++)
			{
				map_role[dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]][j].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定
				map_role[dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]][j].is_room = false;//このデータは通路
			}

			//後の通路を分割線まで伸ばす
			for (size_t j = dungeon_map_role.map_division[room_after][3]; j < dungeon_map_role.map_room[room_after][3]; j++)
			{
				map_role[dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]][j].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定
				map_role[dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]][j].is_room = false;//このデータは通路
			}

			//通路をつなぐ
			for (size_t j = dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]; j <= dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]; j++)
			{
				map_role[j][dungeon_map_role.map_division[room_before][1]].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定
				map_role[j][dungeon_map_role.map_division[room_before][1]].is_room = false;//このデータは通路
			}
			for (size_t j = dungeon_map_role.map_road[room_before][3] + dungeon_map_role.map_room[room_after][2]; j <= dungeon_map_role.map_road[room_before][2] + dungeon_map_role.map_room[room_before][2]; j++)
			{
				map_role[j][dungeon_map_role.map_division[room_before][1]].map_data = static_cast<size_t>(Attribute::Road); //通路の属性に設定
				map_role[j][dungeon_map_role.map_division[room_before][1]].is_room = false;//このデータは通路
			}
			break;

		default:
			break;
		}
	}

	//マップ生成後の処理

	//マスに合わせるため偶数に合わせる

	//部屋の中心を格納する
	for (int i = 0; i <= static_cast<int>(dungeon_map_role.map_room_count); i++)
	{
		const float end_y = static_cast<float>(dungeon_map_role.map_room[i][0]);//部屋のY座標の終点
		const float end_x = static_cast<float>(dungeon_map_role.map_room[i][1]);//部屋のX座標の終点
		const float start_y = static_cast<float>(dungeon_map_role.map_room[i][2]);//部屋のY座標の始点
		const float start_x = static_cast<float>(dungeon_map_role.map_room[i][3]);//部屋のX座標の始点

		//部屋の半分の長さ
		DirectX::XMFLOAT2 room_center_pos{};

		room_center_pos.x = (end_x - start_x) / 2.f;
		room_center_pos.y = (end_y - start_y) / 2.f;

		//部屋の始点を足して、部屋の中心を求める
		room_center_pos.x += start_x;
		room_center_pos.y += start_y;

		if(Math::Comparison(room_center_pos.x,static_cast<int>(room_center_pos.x))==false)//小数点以下を検出
		{
			room_center_pos.x -= 0.5f;
		}
		if (Math::Comparison(room_center_pos.y, static_cast<int>(room_center_pos.y)) == false)//小数点以下を検出
		{
			room_center_pos.y += 0.5f;
		}
		room_center_pos.x *= CellSize;
		room_center_pos.y *= CellSize;
		DirectX::XMINT2 room_center = { static_cast<int>(room_center_pos.x),static_cast<int>(room_center_pos.y) };
		rooms_center.emplace_back(room_center);

	}

	//通路の入り口を格納する
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (map_role[y][x].map_data == static_cast<int>(Attribute::Road))
			{
				DirectX::XMINT2 road = { x * CellSize,y * CellSize };//ワールド座標に合わせるためにCellSizeを積けておく
				roads.emplace_back(road);
			}
		}
	}

	return true;
}

#pragma once
#include <cstdint>
#include <vector>
#include <DirectXMath.h>
#include "Engine/Systems/Math.h"
//参考資料
//https://qiita.com/gis/items/f760a1fb27aa4c3659cf
//https://qiita.com/gis/items/b9cf998db63b465d9f50

#define ONE 1 //DXlibのGetRand関数を使わない場合+1する必要がある

/*マップ系データ*/
const static size_t MapSize_Y = 64; //マップ縦サイズ
const static size_t MapSize_X = 32;   //マップ横サイズ
const static int MapSize = MapSize_Y * MapSize_X;

const static int Mob_Max = 25;//mobの出現最大値
const static int Cell_Size = 2;//world座標での升目の大きさ
/// <summary>
/// mobのマップ情報
/// </summary>
struct MobRole
{
	DirectX::XMFLOAT2 position = {};//マップ上位置
};

/// <summary>
/// マップの雛形
/// </summary>
struct DungeonMapRole
{
	//生成される部屋の数 (正確に言うと生成される区域の数)
	size_t division_count_min = 3; //マップの区分け最小数
	size_t division_count_rand = 4; //マップの区分け数加算

	//生成される部屋のサイズ
	size_t room_length_minX = 5; //部屋のX座標の最小サイズ
	size_t room_length_minY = 5; //部屋のY座標の最小サイズ
	size_t room_length_randX = 2; //部屋のX座標のサイズ加算
	size_t room_length_randY = 2; //部屋のY座標のサイズ加算

	size_t map_division_count = {}; //マップの区分け数 (部屋の個数) 0~nまでの部屋ID
	size_t map_division[8][4] = {}; //マップの区域 [部屋ID][Y終点 , X終点 , Y始点 , X始点]
	size_t map_room_id[8] = {};   //部屋のID
	size_t map_room_count = 0;  //部屋の数
	size_t map_room[8][4] = {}; //マップの部屋 [部屋ID][Y終点 , X終点 , Y始点 , X始点]
	size_t map_road[8][4] = {}; //マップの道 [部屋ID(前)][繋がる先の部屋ID(後) , (0.Y座標 , 1.X座標) , (前)側の通路の位置 , (後)側の通路の位置]
	size_t map_room_player[8] = {};
	size_t map_room_area[8] = { 0 }; //部屋の面積
};

/// <summary>
/// 書き換え可能なマップ
/// </summary>
class RogueLikeMap
{
public:
	RogueLikeMap(const size_t var_) :map_data(var_) {}
	RogueLikeMap() = default;
	//ToDo enum classを作る
	size_t map_data = 1;// 0:壁、1:床、2:プレイヤー、3:敵, 4 : アイテム, 5 : 罠
};

/// <summary>
/// 自作のマップデータを扱う
/// </summary>
class  RogueLikeDungeon
{
public:
	RogueLikeDungeon();
	~RogueLikeDungeon();


	/// <summary>
	/// マップのサイズを初期化する
	/// </summary>
	void InitializeMapSize();

	/// <summary>
	///  インスタンス取得
	/// </summary>
	/// <returns>このクラスの実体</returns>
	//static RogueLikeDungeon& Instance()
	//{
	//	static RogueLikeDungeon instance;
	//	return instance;
	//}

	/// <summary>
	/// マップ生成関数
	/// </summary>
	/// <param name="dungeon_map_role">マップ情報</param>
	/// <returns></returns>
	bool MapMake(DungeonMapRole* const dungeon_map_role/*, std::vector<std::vector<RogueLikeMap>>& map_role*/);

	/// <summary>
	/// マップ情報をもとにマップ上にmobを配置する関数
	/// </summary>
	/// <param name="dungeon_map_role">マップ情報</param>
	/// <param name="id">マップ上のmobに付与するid</param>
	/// <returns></returns>
	bool MobMake(DungeonMapRole* dungeon_map_role, std::vector<std::vector<RogueLikeMap>>& map_role, int id);

	/// <summary>
	/// 新規ダンジョン作成の関数
	/// </summary>
	/// <param name="dungeon_map_role">マップ情報</param>
	void DungeonMake(DungeonMapRole* dungeon_map_role);

	/// <summary>
	/// マップ情報を消去
	/// </summary>
	void MapClear();

	/// <summary>
	/// マップ情報を初期化
	/// </summary>
	void MapReMake(DungeonMapRole* dungeon_map_role);
private:

public:
	/// <summary>
	/// マップの通路の軸
	/// </summary>
	enum class  Road : int
	{
		Axis_Y = 0,
		Axis_X
	};

	/// <summary>
	/// マップ情報を保存するコンテナ
	/// 0:壁、1:床、2:プレイヤー、3:敵, 4:アイテム, 5:罠
	/// </summary>
	std::vector<std::vector<RogueLikeMap>> map_role;
	/// <summary>
	/// マップ情報の雛形　マッププール
	/// </summary>
	DungeonMapRole dungeon_map_role = {};
	/// <summary>
	/// マップ上に存在するmobの情報
	/// </summary>
	MobRole mobs[Mob_Max] = {};
};

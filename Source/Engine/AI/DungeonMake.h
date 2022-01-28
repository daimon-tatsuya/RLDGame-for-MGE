#pragma once
//**********************************************************
//
//		RogueLikeDungeonクラス
//
//**********************************************************
#include <vector>
#include "Engine/Systems/Math.h"

//参考資料
//https://qiita.com/gis/items/f760a1fb27aa4c3659cf
//https://qiita.com/gis/items/b9cf998db63b465d9f50
//http://koyoru1214.blog.fc2.com/blog-entry-7.html
//http://koyoru1214.blog.fc2.com/blog-entry-13.html

// グローバル変数
/*!マップ系データ*/

//XY　最大にする48　これ以上はfpsが低下しだす
static int	MapSize_Y = 48;	// マップ縦サイズ
static int	MapSize_X = 48;   // マップ横サイズ
static int	MapSize = MapSize_Y * MapSize_X;


constexpr static int MobMax = 25;	// mobの出現最大値
constexpr static int RoomMax = 8;	// 部屋の数

constexpr static int	 ONE = 1; // DXlibのGetRand関数を使わない場合+1する必要がある

constexpr static int CellSize = 2;	// world座標での升目の大きさ

//constexpr static int PlayerID = 0;	// プレイヤーのID


//constexpr  static int EndPointY = 0;	// Y終点
//constexpr  static int EndPointX = 1;	// X終点
//constexpr  static int StartPointY = 2;	// Y始点
//constexpr  static int StartPointX = 3;	// X始点


enum class Attribute
{
	Wall = 0,// 0:壁
	Floor,    // 1:床
	Room,	// 2:部屋
	Road,	// 3:道
	Player,   // 4:プレイヤー
	Enemy,  // 5:敵
	Exit,		// 6:階段
	// 6 : アイテム
	// 7 : 罠
	// 8 : アイテムの上のプレイヤー
	// 9 : アイテムの上の敵
	// 10 : 地形の上のプレイヤー
	// 11 :  地形の上の敵
	EndAttribute
};


// 書き換え可能なマップ
struct RogueLikeMap
{
	RogueLikeMap(const size_t var) :map_data(var) {}
	RogueLikeMap() = default;

	size_t map_data = static_cast<size_t>(Attribute::Floor);// マスの属性情報

	//道の入り口かそうでないかのフラグ
	bool road_entrance_flag = false; //  false:道 、true:道の入り口

	//プレーヤーや敵、アイテムが乗っていても潜在的に分かるようするフラグ
	bool is_this_room_flag = false; // false:通路、true:部屋　※壁は部屋ではない扱い(falseのまま)とする。
														//\また、新たに増える場合はこのフラグを消して、enum class で判別するようにする
};


// 自作のマップデータを扱う
class  RogueLikeDungeon
{
private:

public:

	//struct RogueLikeMap
	//{
	//	RogueLikeMap(const size_t var) :map_data(var) {}
	//	RogueLikeMap() = default;

	//	size_t map_data = static_cast<size_t>(Attribute::Floor);// マスの属性情報

	//	//プレーヤーや敵、アイテムが乗っていても潜在的に分かるようするフラグ
	//	bool is_this_room_flag = false; // false:通路、true:部屋　0:1に感覚　※壁は通路扱い(falseのまま)とする。また、新たに増える場合はenum class で判別するようにする
	//};

	enum class  Road : int//  マップの通路の軸
	{
		Axis_Y = 0,
		Axis_X
	};

	// mobのマップ情報
	struct MobRole
	{
		DirectX::XMFLOAT2 position = {};// マップ上位置
		int id{};//マップ上のmobのID
	};


	// マップの雛形
	struct DungeonMapRole
	{
		//生成される部屋の数 (正確に言うと生成される区域の数)
		size_t division_count_min = 4; // マップの区分け最小数
		size_t division_count_rand = 4; // マップの区分け数に加算する最大値
		size_t division_count_max = division_count_rand + division_count_min; // マップの区分け最大

		//生成される部屋のサイズ
		size_t room_length_minX = 5;  // 部屋のX座標の最小サイズ
		size_t room_length_minY = 5;  // 部屋のY座標の最小サイズ
		size_t room_length_randX = 5; // 部屋のX座標のサイズ加算
		size_t room_length_randY = 5; // 部屋のY座標のサイズ加算

		size_t map_division_count{};					// マップの区分け数 (部屋の個数) 0~nまでの部屋ID
		size_t map_division[RoomMax][4]{};		// マップの区域 [部屋ID][Y終点 , X終点 , Y始点 , X始点]
		size_t map_room_id[RoomMax]{};			// 部屋のID
		size_t map_room_count{};						// 部屋の数
		size_t map_room[RoomMax][4]{};			// マップの部屋 [部屋ID][Y終点 , X終点 , Y始点 , X始点]
		size_t map_road[RoomMax][4]{};			// マップの道 [部屋ID(前)] [繋がる先の部屋ID(後) , (0:Y座標 , 1:X座標) , (前)側の通路の位置 , (後)側の通路の位置]
		size_t map_room_area[RoomMax]{};		// 部屋の面積
	};

	std::vector<std::vector<RogueLikeMap>> map_role{};// マップ情報を格納するコンテナ

	std::vector<DirectX::XMINT2>  roads_entrance{};//道の入り口の位置だけを格納するコンテナ

	DungeonMapRole dungeon_map_role{};// マップ情報の雛形　マッププール

	MobRole mobs[MobMax]{};// マップ上に存在するmobの情報

	size_t map_room_player{};	// Playerのいる部屋の番号


private:

public:

	RogueLikeDungeon();
	~RogueLikeDungeon();

	//マップのサイズを初期化する
	void InitializeMapSize();

	// 敵の更新の前にプレイヤーのマップ情報を更新する
	void UpdateMapRolePlayer();

	// 敵のマップ情報を更新する
	void UpdateMapRoleEnemis();

	// マップ生成関数
	bool MapMake();

	/// <summary>
	/// マップ情報をもとにマップ上にmobを配置する関数
	/// </summary>
	/// <param name="id">マップ上のmobに付与するid</param>
	/// <returns></returns>
	bool MobMake(int id);

	//新規ダンジョン作成の関数
	void DungeonMake();


	// マップ情報を消去
	void MapClear();

	// マップ情報を初期化
	void MapReMake();

	// デバッグ用GUI描画
	void DrawDebugGUI();

};

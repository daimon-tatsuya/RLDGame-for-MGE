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

//XYそれぞれ最大48にする　49以上の値はfpsが低下しだす
static int	MapSize_Y = 48;	// マップ縦サイズ
static int	MapSize_X = 48;   // マップ横サイズ
static int	MapSize = MapSize_Y * MapSize_X;

const static int ObjectMax = 40;	// オブジェクトの出現最大値
const static int RoomMax = 8;	// 部屋の数
const static int	 ONE = 1; // DXlibのGetRand関数を使わない場合+1する必要がある
const static int CellSize = 2;	// world座標での升目の大きさ

	// マップの情報の雛型
	struct DungeonMapRole final
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
		size_t map_road[RoomMax][4]{};			// マップの通路 [部屋ID(前)] [繋がる先の部屋ID(後) , (0:Y座標 , 1:X座標) , (前)側の通路の位置 , (後)側の通路の位置]
		size_t map_room_area[RoomMax]{};		// 部屋の面積
	};

///<summary>
///マップ情報の属性
///</summary>
///<para>
///床のオブジェクト通路より小さい値にする
///</para>
enum class Attribute
{
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
};

// 書き換え可能なマップ
struct RogueLikeMap final
{
	RogueLikeMap(const size_t var) :map_data(var) {}
	RogueLikeMap() = default;

	size_t map_data = static_cast<size_t>(Attribute::Wall);// マスの属性情報

	//通路の入り口かそうでないかのフラグ
	bool is_road_entrance = false; //  false:通路 、true:通路の入り口

	//プレーヤーや敵、アイテムが乗っていても潜在的に分かるようするフラグ
	bool is_room = false; // false:通路、true:部屋　※壁は部屋ではない扱い(falseのまま)とする。
	//\また、新たに増える場合はこのフラグを消して、enum class で判別するようにする
};

// 自作のマップデータを扱う
class  RogueLikeDungeon final
{
private:

	//static RogueLikeDungeon* instance;//	唯一のインスタンス

	std::vector<std::vector<RogueLikeMap>> map_role{};// マップ情報を格納するコンテナ

	std::vector<DirectX::XMFLOAT2>  roads_entrance{};//通路の入り口の位置だけを格納するコンテナ

public:

	enum class  Road : int//  マップの通路の軸
	{
		Axis_Y = 0,
		Axis_X
	};

	DungeonMapRole dungeon_map_role{};// マップ情報の雛形　マッププール

	size_t map_room_player{};	// プレイヤーのいる部屋の番号

	//上書き防止保存用
	DirectX::XMINT2 player_pos{};
	DirectX::XMINT2 stairs_pos{};

private:

	//マップのサイズを初期化する
	void InitializeMapSize();


	// マップ生成を行う
	void MakeMap();

	/// <summary>
	/// マップ情報をもとにマップ上にobjectの位置を設定する
	/// </summary>
	/// <param name="id">マップ上のobjectに付与するid</param>
	/// <returns></returns>
	void SetObjectPos(int id);

	// マップ情報を消去
	void ClearMap();


public:

	// 唯一のインスタンス取得
	static RogueLikeDungeon& Instance()
	{
		// インスタンス設定
		static RogueLikeDungeon instance;
		return instance;
	}

	//コンストラクタ
	RogueLikeDungeon();

	//デストラクタ
	~RogueLikeDungeon();

	//マップ情報を更新する
	void UpdateMapRole();

	//新規ダンジョン作成を行う
	void MakeDungeon();

	// デバッグ用GUI描画
	void DrawDebugGUI() const;

	std::vector<std::vector<RogueLikeMap>> GetMapRole() { return  map_role; }

	std::vector<DirectX::XMFLOAT2> GetRoadsEntrance() { return roads_entrance; }
};

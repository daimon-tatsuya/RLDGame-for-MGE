#pragma once
//**********************************************************
//
//		DungeonSystemクラス
//
//**********************************************************

///<summary>
/// ダンジョンのシステム管理クラス
///</summary>
class DungeonSystem final
{
private:
	int elapsed_turn = 0;	//経過ターン 敵の行動が終わるたびに1プラスする
	int max_turn = 999;		//最大経過ターン 経過ターンがこの値を超えたら

	int current_floor = 1;//現在の階数
	int max_floor = 4;   //最大階層

	bool is_next_floor = false;//次のフロアに進むならtrue

	bool is_dungeon_clear = false;//ダンジョンをクリアをしたらtrue
public:

	//	コンストラクタ
	DungeonSystem();

	//デストラクタ
	~DungeonSystem() = default;

	// 唯一のインスタンス取得
	static DungeonSystem& Instance()
	{
		static DungeonSystem instance;
		return instance;
	}

	//初期化関数
	void Initialize();


	void Initialize(int max_floor,int max_turn);

	//現在の経過ターンが最大経過ターンを超えているかのチェック
	bool  ExceededMaxTurn() const;

	//現在の階数が最大階数に到達しているかのチェック
	bool  BreakingThroughTopFloor() const;

	//ターンを経過させる
	void ElapseTurns() { ++elapsed_turn; }

	//--------------------------------------------
	//	 Getter Setter
	//--------------------------------------------

	//最大階層の設定
	void SetMaxFloor(int max) { max_floor = max; }

	//最大階層の取得
	int GetMaxFloor() const { return max_floor; }

	//現在の階数を取得
	int GetCurrentFloor()const { return  current_floor; }

	//階を進ませる
	void ElapseCurrentFloor() { ++current_floor; }

	//経過しているターンを取得
	int GetElapsedTurn() const { return elapsed_turn; }

	//最大経過ターンの設定
	void SetMaxTurn(int max) { max_turn = max; }

	//最大経過ターンの取得
	int GetMaxTurn() const { return max_turn; }

	//ダンジョンクリアのフラグの設定
	void SetIsDungeonClear(bool set_flag) { is_dungeon_clear = set_flag; }

	//ダンジョンクリアのフラグの取得
	bool GetIsDungeonClear() const { return is_dungeon_clear; }

	//次の階に進むのフラグの設定
	void SetIsNextFloor(bool set_flag) { is_next_floor = set_flag; }

	//次の階に進むのフラグの取得
	bool GetIsNextFloor() const { return is_next_floor; }

};



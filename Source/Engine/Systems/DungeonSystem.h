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
	int max_floor = 99;   //最大階数

	static  DungeonSystem* instance;//唯一のポインタ

public:

	//	コンストラクタ
	DungeonSystem();

	//デストラクタ
	~DungeonSystem() = default;

	//唯一のインスタンス
	static DungeonSystem& Instance() { return *instance; }

	//ターンを経過させる
	void TurnsElapse() { ++elapsed_turn; }

	//経過しているターンを取得
	int GetElapsedTurn() const { return elapsed_turn; }

	//最大経過ターンの設定
	void SetMaxTurn(int max) { max_turn = max; }
};



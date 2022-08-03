#pragma once

//**********************************************************
//
//		EnemyManagerクラス
//
//**********************************************************


#include <vector>

//前方宣言
class Character;

class EnemyManager
{
private:

	std::vector<Character*>	 enemies;// エネミーだけを格納する
	std::vector<Character*>	 removes; // 削除するEnemy(Character)を格納するして, characteresのindexを指定して直接削除するのを回避
public:

private:

	EnemyManager() = default;
	~EnemyManager();

public:

	//EnemyManagerの生成と削除は全てCharacterManagerから行う
	 friend class CharacterManager;

	// 敵のを全削除
	void Clear();

	/// <summary>
	/// 敵を登録
	/// </summary>
	/// <param name="character">登録するキャラ</param>
	void Register(Character* character);

	/// <summary>
	/// 敵を削除
	/// </summary>
	/// <param name="character">削除するキャラ</param>
	void Remove(Character* character);

//------------------------------------------------
//
// Getter
//
//------------------------------------------------

	// 敵を数取得
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// 敵配列を取得
	std::vector<Character*> GetEnemis() { return enemies; }

	// 敵を取得
	Character* GetEnemy(int index) { return enemies.at(index); }

	// IDから敵を取得
	Character* GetEnemyFromId(int id);

};

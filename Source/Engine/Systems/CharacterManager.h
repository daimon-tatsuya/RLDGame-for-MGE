#pragma once
//**********************************************************
//
//	   CharacterManagerクラス
//
//**********************************************************

#include <vector>
#include<memory>
#include <d3d11.h>
#include "Engine/AI/Telegram.h"

//前方宣言
class Shader;
class Character;

/// <summary>
/// Characterを管理するクラス
/// </summary>
class CharacterManager final
{
private:

	std::vector<std::shared_ptr<Character>>	  characters;			// 敵味方関係なく格納する
	std::vector<std::shared_ptr<Character>>	  removes;				// 削除するCharacterを格納するして,charactersのindexを指定して直接削除するのを回避

	int enemy_number = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)
	int team_number = 0;	// 付与するIDの値(この値にMetaAI::Identity::Teamを加算して付与する)

	//bool	is_player_turn = true;//プレイヤーのターンのときにtrueになって、更新関数が実行される
	//bool	is_enemy_turn = false;//敵のターンのときにtrueになって、更新関数が実行される

public:

private:
	//コンストラクタ
	CharacterManager() = default;
	//デストラクタ
	~CharacterManager() = default;
public:

	// 唯一のインスタンス取得
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// キャラクター同士の衝突処理
	void CollisionCharacterToCharacter() const;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsed_time"></param>
	void Update(float elapsed_time);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dc">DeviceContext</param>
	/// <param name="shader">描画の仕方</param>
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) const;

	// キャラクターのを全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() const;

	// デバッグ用GUI描画
	void DrawDebugGUI() const;

	/// <summary>
	/// キャラクターを登録
	/// </summary>
	/// <param name="character">登録するキャラクター</param>
	/// <param name="character_type">Identity</param>
	void Register(Character* character, int character_type);

	/// <summary>
	/// メッセージ受信処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns>受信の有無</returns>
	bool OnMessage(const Telegram& telegram);

	/// <summary>
	/// キャラクターを削除
	/// </summary>
	/// <param name="character">削除するキャラクター</param>
	void Remove(Character* character);

	/// <summary>
   /// 敵キャラクターを全削除
   /// </summary>
	void RemoveEnemy() const;

	//------------------------------------------------
	//
	// Getter
	//
	//------------------------------------------------

	/// <summary>
	/// IDからキャラクターを取得
	/// </summary>
	/// <param name="id">取得するキャラクターのID</param>
	/// <returns></returns>
	Character* GetCharacterFromId(int id) const;

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	/// <returns>Player</returns>
	Character* GetPlayer() const;

	// キャラクターを数取得
	int GetCharacterCount() const { return static_cast<int>(characters.size()); }

	//　index 番目のキャラクターを取得
	Character* GetCharacter(int index) const { return characters.at(index).get(); }

	//キャラクターのコンテナを取得
	std::vector<std::shared_ptr<Character>> GetCharacters() const { return characters; }

	//敵の数を取得
	int GetEnemyCount() const { return  enemy_number; }

	//敵の中の index 番目のIDの敵を取得
	Character* GetEnemy(int index) const;
};

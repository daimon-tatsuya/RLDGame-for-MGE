#pragma once

//**********************************************************
//
//	CharacterManagerクラス
//
//**********************************************************

#include <vector>
#include<memory>
#include <d3d11.h>



//前方宣言
class Shader;
class Character;

/// <summary>
/// Characterを管理するクラス
/// </summary>
class CharacterManager
{
private:

	std::vector<std::shared_ptr<Character>>	  characteres;			// 敵味方関係なく格納する
	std::vector<std::shared_ptr<Character>>	  removes;				// 削除するCharacterを格納するして,characteresのindexを指定して直接削除するのを回避
	int									  enemy_number = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)
	int									  player_number = 0;	// 付与するIDの値(この値にMetaAI::Identity::Playerを加算して付与する)

public:

private:
	CharacterManager() {}
	~CharacterManager(){}
public:

	// 唯一のインスタンス取得
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// キャラクター同士の衝突処理
	void CollisionCharacterToCharacter();

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
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// キャラクターのを全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

	/// <summary>
	/// キャラクターを登録
	/// </summary>
	/// <param name="character">登録するキャラクター</param>
	/// <param name="character_type">Meta::Identity</param>
	void Register(Character* character, int character_type);

	/// <summary>
	/// キャラクターを削除
	/// </summary>
	/// <param name="character">削除するキャラクター</param>
	void Remove(Character* character);

	/// <summary>
	/// IDからキャラクターを取得
	/// </summary>
	/// <param name="id">取得するキャラクターのID</param>
	/// <returns></returns>
	Character* GetCharacterFromId(int id);

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	/// <param name="number">プレイヤーの番号:0~3</param>
	/// <returns>number番目のPlayer</returns>
	Character* GetPlayer(int number = 0);

	//------------------------------------------------
	//
	// Getter
	//
	//------------------------------------------------

	// キャラクターを数取得
	int GetCharacterCount() const { return static_cast<int>(characteres.size()); }

	//　index 番目のャラクターを取得
	Character* GetCharacter(int index) { return characteres.at(index).get(); }

	//敵の数を取得
	int GetEnemyCount() const { return  enemy_number; }

	// index 番目に最も近いIDの敵を取得
	Character* GetEnemy(int index);

};

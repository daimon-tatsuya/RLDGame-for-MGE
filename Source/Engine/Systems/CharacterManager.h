#pragma

#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/EnemyBase.h"
#include "Engine/Systems/PlayerBase.h"
//ToDo CharacterManager コメント

class CharacterManager
{
private:

	std::vector<Character*>	 characteres;
	std::vector<Character*>	 removes;
	std::vector<Character*>  enemies;// エネミーだけを格納する
	int enemy_number = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算して付与する)
	int player_number = 0; // 付与するIDの値(この値にMetaAI::Identity::Playerを加算して付与する)

public:

private:

	CharacterManager() {}
	~CharacterManager();

	// キャラクター同士の衝突処理
	void CollisionCharacterToCharacter();

public:

	// 唯一のインスタンス取得
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// キャラクターのを全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

	// キャラクターを登録
	void Register(Character* character, int character_type);

	// キャラクターを削除
	void Remove(Character* character);

	// キャラクターを数取得
	int GetCharacterCount() const { return static_cast<int>(characteres.size()); }

	// キャラクターを取得
	Character* GetCharacter(int index) { return characteres.at(index); }

	// キャラクターを数取得
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// 敵配列を取得
	std::vector<Character*> GetEnemis() { return enemies; }

	// 敵を取得
	Character* GetEnemy(int index) { return enemies.at(index); }

	//IDからキャラクターを取得
	Character* GetCharacterFromId(int id);

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	/// <param name="number">プレイヤーの番号:0~3</param>
	/// <returns>number番目のPlayer</returns>
	Character* GetPlayer(int number = 0);
};

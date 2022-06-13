#pragma once
//**********************************************************
//
//		SceneManagerクラス
//
//**********************************************************



//前方宣言
class Scene;

/// <summary>
/// シーンを管理するクラス
/// </summary>
class SceneManager final
{
private:

	Scene* current_scene = nullptr;	//現在のシーン
	bool is_game_clear = false;//ゲームをクリアしたならtrue
public:
private:
	SceneManager() = default;

	~SceneManager();

public:
	// 唯一のインスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time) const;

	// 描画処理
	void Render() const;

	// シーンクリア
	void Clear();

	// シーン切り替え
	void ChangeScene(Scene* scene);

	//現在のシーンを取得
	Scene* GetCurrentScene() const { return current_scene; }

	//ゲームをクリアしているかのフラグを取得
	bool GetIsGameClear()const { return  is_game_clear; }

	//ゲームをクリアしているかのフラグを取得
	void SetIsGameClear(bool flag) {   is_game_clear=flag; }

};
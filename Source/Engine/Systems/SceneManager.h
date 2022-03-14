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
class SceneManager
{
private:

	Scene* current_scene = nullptr;

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
};
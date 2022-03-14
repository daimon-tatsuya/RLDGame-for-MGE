#pragma once
//**********************************************************
//
//		Scenesクラス
//
//**********************************************************

/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
private:

	bool	ready = false;

public:

private:

public:

	Scene() = default;
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Scene() = default;

	// 初期化
	virtual void Initialize() = 0;
	// 終了化
	virtual void Finalize() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render() = 0;
	// 準備完了しているか
	bool IsReady() const { return ready; }
	// 準備完了設定
	void SetReady() { ready = true; }
};

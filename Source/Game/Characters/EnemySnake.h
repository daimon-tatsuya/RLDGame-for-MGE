#pragma once
//**********************************************************
//
//		EnemySnakeクラス
//
//**********************************************************



#include "Engine/Systems/EnemyBase.h"

/// <summary>
/// 敵クラス(ヘビ)
/// </summary>
class EnemySnake :
	public EnemyBase
{
private:

protected:

public:

	//親ステート
	enum class ParentState :int
	{
		Entry = 0,
		Reaction,
		Receive,

		StateEnd
	};

	//子ステート
	enum class Entry :int
	{
		Select = 0,
		Approach,
		Explore,
		Attack,
		Ability,
		UseItem,

		StateEnd
	};

	//子ステート
	enum class Reaction :int
	{
		Damaged = 0,
		Death,

		StateEnd
	};

	//子ステート
	enum class Receive
	{
		Wait,
		Called,
		StateEnd
	};


private:


protected:


public:

	EnemySnake(RogueLikeDungeon* rogue_like_dungeon);
	virtual ~EnemySnake();

	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//有限ステートマシンの初期化
	void FSMInitialize() override;

	// 破棄
	void Destroy();

	// デバッグエネミー情報表示
	void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged()override;

	// 死亡した時に呼ばれる
	void OnDead()override;

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) override;

};

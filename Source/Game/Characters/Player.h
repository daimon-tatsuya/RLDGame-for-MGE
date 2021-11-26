#pragma once
//**********************************************************
//
//		Playerクラス
//
//**********************************************************

#include "Engine/Systems/Character.h"
#include "Engine/Systems/DungeonMake.h"

class Player :
	public Character
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
		WayChange,
		Move,
		Attack,
		Menu,
		//今後作る予定のステート
		//Map,
		//Ability,
		//MessageLog,
		//Inventry,
		//View,
		//Step,

		StateEnd
	};

	//子ステート
	enum class Reaction :int
	{
		Damaged=0,
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

	Player(RogueLikeDungeon* rogue_like_dungeon);
	~Player()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dc"></param>
	/// <param name="shader">描画方法</param>
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//有限ステートマシンの初期化
	void FSMInitialize() override;

	// デバッグ用GUI描画
	void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	//メッセージ受信処理
	bool OnMessage(const Telegram& msg) override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged() override;

	// 死亡した時に呼ばれる
	void OnDead() override;

};

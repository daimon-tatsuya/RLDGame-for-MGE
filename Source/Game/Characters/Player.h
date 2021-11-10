#pragma once

#include "Engine/Systems/PlayerBase.h"
#include "Engine/Systems/DungeonMake.h"

//ToDo Playerが行うことを書く


class Player :
	public PlayerBase
{
private:

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
		//Map,
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
		Called,
		StateEnd
	};
private:

public:

	Player(RogueLikeDungeon* rogue_like_dungeon);
	Player();
	~Player()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//有限ステートマシンの初期化
	void FSMInitialize() override;

	// デバッグ用GUI描画
	void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	//メッセージ受信処理
	bool OnMessage(const Telegram& msg) override;

	// 着地した時に呼ばれる
	void OnLanding() override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged() override;

	// 死亡した時に呼ばれる
	void OnDead() override;


	//Todo 左右上下斜めの方向に一定の大きさ進む
	//bool InputMove(float elapsedTime) override;


};

//**********************************************************
//
//		DungeonSystemクラス
//
//**********************************************************

#include "Engine/Systems/DungeonSystem.h"
#include <crtdbg.h>

DungeonSystem* DungeonSystem::instance = nullptr;//唯一のポインタの初期化

//コンストラクタ
DungeonSystem::DungeonSystem()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

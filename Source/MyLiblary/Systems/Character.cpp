//**********************************************************
//
//		Characterクラス
//
//**********************************************************
#include "Engine/Systems/Character.h"
#include "Engine/Systems/CharacterManager.h"
//デストラクタ
Character::~Character()
{
	//最短経路の破棄
	if (shortest_path!=nullptr)
	{
		shortest_path->Clear();
		delete shortest_path;
	}
}


void Character::Destroy()
{
	//キャラクターマネージャーの消去リストに自信を追加
	CharacterManager::Instance().Remove(this);
}

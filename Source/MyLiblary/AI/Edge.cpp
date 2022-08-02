//**********************************************************
//
// 	Edgeクラス
//
//**********************************************************

// ヘッダー
#include "Engine/AI/Edge.h"

// 初期化
void Edge::Initialize(const int node, int& destination)
{
	origin_node_id = node;
	destination = -1;
}
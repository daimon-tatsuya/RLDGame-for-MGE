//**********************************************************
//
//		Nodeクラス
//
//**********************************************************

#include "Engine/AI/Node.h"
#include "Engine/AI/Edge.h"

Node::Node()
{
	//edge配列のサイズ初期化
	for (int i = 0; i < EDGE_NUM; i++)
	{
		auto o = std::make_shared<Edge>();
		edge.emplace_back(o);
	}
}
//**********************************************************
//
//		Node�N���X
//
//**********************************************************

#include "Engine/AI/Node.h"
#include "Engine/AI/Edge.h"

Node::Node()
{
	//edge�z��̃T�C�Y������
	for (int i = 0; i < EDGE_NUM; i++)
	{
		auto o = std::make_shared<Edge>();
		edge.emplace_back(o);
	}
}
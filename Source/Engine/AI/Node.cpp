//**********************************************************
//
//		NodeƒNƒ‰ƒX
//
//**********************************************************

#include "Engine/AI/Node.h"
#include "Engine/AI/Edge.h"


Node::Node()
{
	for (int i = 0; i < EDGE_NUM; i++)
	{
		auto o = std::make_shared<Edge>();
		edge.emplace_back(o);
	}
}
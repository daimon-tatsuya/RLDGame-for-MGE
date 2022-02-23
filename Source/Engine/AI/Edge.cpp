//**********************************************************
//
// 	EdgeƒNƒ‰ƒX
//
//**********************************************************

#include "Engine/AI/Edge.h"

void Edge::Initialize(const int node, int& destination)
{
	origin_node_id = node;
	destination = -1;
}
#include "Engine/AI/Node.h"

Node::Node()
{
	for (int i = 0; i < EDGE_NUM; i++)
	{
		auto o = std::make_shared<Edge>();
		edge.push_back(o);
	}
}

Node::~Node()
{

}
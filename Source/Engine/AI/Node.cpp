//**********************************************************
//
//		Node�N���X
//
//**********************************************************

//�w�b�_�[
#include "Engine/AI/Node.h"
#include "Engine/AI/Edge.h"

//	�R���X�g���N�^
Node::Node()
{
	//edge�z��̃T�C�Y������
	for (int i = 0; i < EDGE_NUM; i++)
	{
		auto o = std::make_shared<Edge>();
		edge.emplace_back(o);
	}
}
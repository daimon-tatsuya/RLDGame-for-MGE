//**********************************************************
//
//		Node�N���X
//
//**********************************************************

//�w�b�_�[
#include "Liblary/AI/Node.h"
#include "Liblary/AI/Edge.h"

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
#pragma once

//**********************************************************
//
//		Edge�N���X
//
//**********************************************************



//0,  /*�G�b�W�̕\���@��̕���*/
//1,  /*�G�b�W�̕\���@�E��̕���*/
//2,  /*�G�b�W�̕\���@�E�̕���*/
//3,  /*�G�b�W�̕\���@�E���̕���*/
//4,  /*�G�b�W�̕\���@���̕���*/
//5,  /*�G�b�W�̕\���@�����̕���*/
//6,  /*�G�b�W�̕\���@���̕���*/
//7,  /*�G�b�W�̕\���@����̕���*/
//�@�@�@7     0      1
//  �@�@  �_  |   /
//�@�@6    -  �E  -   2
//�@�@�@  /�@|   �_
//    �@ 5      4�@�@ 3
enum class EdgeDirection :int
{
	TopCenter = 0,
	TopRight,
	CenterRight,
	BottomRight,
	BottomCenter,
	BottomLeft,
	CenterLeft,
	TopLeft
};

const int EDGE_NUM = 8;//�G�b�W�̐��@���@�����̐�


class Edge
{
private:

public:

	int distnation_node = 0;//�ڑ���m�[�hID
	int origin_node = 0;//���m�[�hID
	float cost = 1.0f;  //�R�X�g

private:

public:

	Edge() {}
	~Edge() {}

	//������
	void Initialize(int node, int& buffer);

};

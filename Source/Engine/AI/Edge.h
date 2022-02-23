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
//�@�@6   -  �E  -   2
//�@�@�@  /�@|   �_
//    �@ 5      4�@�@3
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

//constexpr constant expression(�萔��)�̗�
//https://qiita.com/saltheads/items/dd65935878a0901fe9e7

constexpr int EDGE_NUM = 8;// �G�b�W�̐��@���@�����̐�


class Edge
{
private:

public:

	int destination_node_id = 0;// �ڑ���m�[�hID
	int origin_node_id = 0;		// ���m�[�hID
	float cost = 1.0f;			// �R�X�g

private:

public:

	Edge() = default;
	~Edge() = default;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="node">�m�[�hID</param>
	/// <param name="destination">�ڑ���</param>

	void Initialize(const int node, int& destination);
};

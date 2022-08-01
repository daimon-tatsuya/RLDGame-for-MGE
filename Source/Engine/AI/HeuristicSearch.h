#pragma once
//**********************************************************
//
//	 HeuristicSearch�N���X
//
//**********************************************************

// �w�b�_�[
#include <vector>
#include <memory>
#include "Engine/Systems/Math.h"

// �O���錾
class Node;
class Edge;
class RogueLikeDungeon;

// �ŒZ�o�H
struct ShortestPath
{
	DirectX::XMFLOAT3 destination_pos{};// �ړI�n�̃}�b�v��̈ʒu
	std::vector<int> path_information{};// �ŒZ�o�H
	unsigned int path_index{};// �ŒZ�o�H�p�̔z��index�ԍ�
	unsigned int path_size;// �ŒZ�o�H�̃R���e�i�̃T�C�Y�Apath_index-1�Ɣ�r���ē����Ȃ�

	// �����o�̏�����
	void Clear()
	{
		destination_pos = DirectX::XMFLOAT3(0, 0,0);
		path_information.clear();
		path_information.shrink_to_fit();
		path_index = 0;
		path_size = 0;
	}

	// �ŒZ�o�H�̃T�C�Y��ݒ�
	void SetPathSize() { path_size = static_cast<unsigned int>(path_information.size()); }

};

/// <summary>
/// A*�̌v�Z���s��
/// </summary>
class HeuristicSearch final
{
private:

public:

	const int ASTER_COST_PERCENTAGE = 20;		// A*�̃R�X�g�v�Z���Ɋ���l

	std::vector<std::shared_ptr<Node>> nodes;		// �S�m�[�h��ێ�����x�N�^�[

	std::vector<std::shared_ptr<Edge>> searched_edge;// �T���ς݂̃G�b�W��ۑ�

	std::vector<int>  advance;				// �T���������ʍŒZ�̃��[�g�A�i�s���[�g�̃G�b�W���L������B

	static HeuristicSearch* instance;	// �B��̃C���X�^���X

	std::vector<int>  map_info;			// �}�b�v�����ꎟ���s�񉻂����R���e�i

	//std::vector<std::vector<int>>advance_vector;

private:

	/// <summary>
	/// ���(candidate)����R�X�g���l�����čŒZ�G�b�W����Ԃ�
	/// </summary>
	/// <param name="candidate">�T�����̔z��</param>
	/// <param name="now_edge">���ݒT������edge</param>
	/// <param name="goal_id"></param>
	/// <returns>�T�������̍ŏ��R�X�g��edge���</returns>
	std::shared_ptr<Edge> SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> now_edge, int goal_id);

	/// <summary>
	/// �Q�̃m�[�h�̕����������o���Ă��̋�����Ԃ�
	/// </summary>
	/// <param name="n1">���݂̃m�[�h</param>
	/// <param name="n2">�T����(�S�[��)�̃m�[�h</param>
	/// <returns>����/20</returns>
	static float HeuristicCalculation(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

public:

	//�R���X�g���N�^
	HeuristicSearch();

	//�f�X�g���N�^
	~HeuristicSearch();

	// �B��̃C���X�^���X�擾
	static HeuristicSearch& Instance() { return *instance; }

	/// <summary>
	/// nodes�̏�����
	/// </summary>
	void NodesInitialize();

	/// <summary>
	/// start_id����goal_id�̃R�X�g��}�����ŒZ�o�H���v�Z
	/// </summary>
	/// <param name="start_id">�}�b�v�����������̃X�^�\�g�n�_</param>
	/// <param name="goal_id">�}�b�v�����������̃X�^�\�g�n�_</param>
	/// <returns>�ŒZ�o�H�̈ʒu</returns>
	bool Search(int start_id, int goal_id);

	//---------------------------
	// Convert Method
	//---------------------------

	/// <summary>
	/// �ꎟ���z����}�b�v���ɕϊ�
	/// </summary>
	/// <para>�}�b�v���ϊ��֐�</para>
	/// <param name="one_dimensional">�ꎟ���z��ɕϊ�����ʒu</param>
	/// <returns>�ϊ���̒l</returns>
	DirectX::XMINT2 ConvertMapPosition(int one_dimensional);

	/// <summary>
	/// ���[���h���W(int x,int z)��1�����ɕϊ�����
	/// </summary>
	/// <para>�}�b�v���ϊ��֐�</para>
	/// <param name="world">�ꎟ���z��ɕϊ�����ʒu</param>
	/// <returns>�ϊ���̒l</returns>
	int ConvertWorldToOneDimensional(DirectX::XMFLOAT2 world);
	int ConvertWorldToOneDimensional(DirectX::XMINT2 world);

	/// <summary>
	/// �}�b�v�����ꎟ���z��ɕϊ�����
	/// </summary>
	/// <para>�}�b�v���ϊ��֐�</para>
	void ConvertMapInformationToOneDimensionalArray();
};

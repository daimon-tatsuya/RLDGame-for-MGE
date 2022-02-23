#pragma once
//**********************************************************
//
//	HeuristicSearch�N���X
//
//**********************************************************

#include <vector>
#include <memory>
#include "Engine/Systems/Math.h"
// �O���錾
class Node;
class Edge;
class RogueLikeDungeon;

struct ShortestPath
{
	DirectX::XMINT2 destination_pos{};//�ړI�n�̃}�b�v��̈ʒu
	std::vector<int> path{};//�ŒZ�o�H
	unsigned int path_index{};//�ŒZ�o�H�p�̔z��index�ԍ�

	void Clear()
	{
		destination_pos = DirectX::XMINT2(0,0);
		path.clear();
		path_index = 0;
	}

	//�����o�̏�����
};

class HeuristicSearch final
{
private:

public:
	const	int AsterCostPercentage = 20;// A*�̃R�X�g�v�Z���Ɋ���l
	std::vector<std::shared_ptr<Node>> nodes;// �S�m�[�h��ێ�����x�N�^�[
	std::vector<std::shared_ptr<Edge>> searched_edge;// �T���ς݂̃G�b�W��ۑ�
	std::vector<int>  advance;	// �i�s���[�g�̃G�b�W���L������
	static HeuristicSearch* instance;// �B��̃C���X�^���X

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
	float HeuristicCalculation(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

public:

	HeuristicSearch();
	~HeuristicSearch();

	// �B��̃C���X�^���X�擾
	static HeuristicSearch& Instance() { return *instance; }

	/// <summary>
	/// nodes�̏�����
	/// </summary>
	/// <param name="rogue_like_dungeon">�}�b�v���</param>
	void Reset(const RogueLikeDungeon& rogue_like_dungeon);

	/// <summary>
	/// start_id����goal_id�̃R�X�g��}�����ŒZ�o�H���v�Z
	/// </summary>
	/// <param name="start_id">�}�b�v�����������̃X�^�\�g�n�_</param>
	/// <param name="goal_id">�}�b�v�����������̃X�^�\�g�n�_</param>
	/// /// <param name="rogue_like_dungeon">�}�b�v���</param>
	/// <returns>�ŒZ�o�H</returns>
	std::vector<int> Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon);


};

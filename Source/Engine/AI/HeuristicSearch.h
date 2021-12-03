#pragma once

#include <stack>
#include <queue>
#include <list>
#include "Engine/AI/Edge.h"
#include "Engine/AI/Node.h"
#include "Engine/Systems/DungeonMake.h"


class HeuristicSearch final
{
private:

public:

	std::vector<std::shared_ptr<Node>> nodes;//�S�m�[�h��ێ�����x�N�^�[

	std::vector<std::shared_ptr<Edge>> searched_edge;//�T���ς݂̃G�b�W��ۑ�

	std::vector<int>  advance;//�i�s���[�g�̃G�b�W�����L������

	static HeuristicSearch* instance;//�B��̃C���X�^���X

    const	int AsterCostPercentage = 20;//A*�̃R�X�g�v�Z���Ɋ���l
private:
	/// <summary>
	/// candidate����R�X�g���l�����čŒZ�G�b�W����Ԃ�
	/// </summary>
	/// <param name="candidate">�T�����̔z��</param>
	/// <param name="nowEdge">���ݒT������edge</param>
	/// <returns>�T�������̍ŏ��R�X�g��edge���</returns>
	std::shared_ptr<Edge> SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> nowEdge, int goal_id);

public:

	HeuristicSearch();
	~HeuristicSearch();

	//�B��̃C���X�^���X�擾
	static HeuristicSearch& Instance() { return *instance; };


	void Reset(const RogueLikeDungeon& rogue_like_dungeon);

	/// <summary>
	/// start_id����goal_id�̃R�X�g��}�����ŒZ�o�H���v�Z
	/// </summary>
	/// <param name="start_id">�}�b�v�����������̃X�^�\�g�n�_</param>
	/// <param name="goal_id">�}�b�v�����������̃X�^�\�g�n�_</param>
	/// /// <param name="rogue_like_dungeon">�}�b�v���</param>
	/// <returns>�ŒZ�o�H</returns>
	std::vector<int> Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon);

	/// <summary>
	/// //�Q�̃m�[�h�̕����������o���Ă��̋�����Ԃ�
	/// </summary>
	/// <param name="N1">���݂̃m�[�h</param>
	/// <param name="N2">�T����(�S�[��)�̃m�[�h</param>
	/// <returns>����/20</returns>
	float heuristicCulc(std::shared_ptr<Node> N1, std::shared_ptr<Node> N2);
};

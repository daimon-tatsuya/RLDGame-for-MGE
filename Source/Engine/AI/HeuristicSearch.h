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
	/// <summary>
	/// candidate����R�X�g���l�����čŒZ�G�b�W����Ԃ�
	/// </summary>
	/// <param name="candidate">�T�����̔z��</param>
	/// <param name="nowEdge">���ݒT������edge</param>
	/// <returns>�T�������̍ŏ��R�X�g��edge���</returns>
	std::shared_ptr<Edge> SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> nowEdge, int goal_id);

public:

	HeuristicSearch() {};
	HeuristicSearch(const RogueLikeDungeon& rogue_like_dungeon);
	~HeuristicSearch();

	std::vector<int> Search(int start_id, int goal_id);

	static HeuristicSearch& Instance();

public:
	//�S�m�[�h��ێ�����x�N�^�[
	std::vector<std::shared_ptr<Node>> nodes;
	//�T���ς݂̃G�b�W��ۑ�
	std::vector<std::shared_ptr<Edge>> searched_edge;
	//�i�s���[�g�̃G�b�W�����L������
	std::vector<int>  advance;
};

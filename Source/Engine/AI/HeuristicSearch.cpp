#include "Engine/AI/HeuristicSearch.h"
#include "Engine/Systems/DungeonMake.h"

 HeuristicSearch* HeuristicSearch::instance = nullptr;

HeuristicSearch::HeuristicSearch()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

HeuristicSearch::~HeuristicSearch()
{
	if (advance.size())
	{
		advance.clear();
	}
}

void HeuristicSearch::Reset(const RogueLikeDungeon& rogue_like_dungeon)
{
	//node�̏�����
	searched_edge.clear();
	int node_id = 0;
	for (int y = 0; y < static_cast<int>(MapSize_Y); y++)
	{
		for (int x = 0; x < static_cast<int>(MapSize_X); x++)
		{
			auto piece = std::make_shared<Node>();
			piece->node_id = node_id;
			piece->SetNodePosition(static_cast<float>(x * Cell_Size), static_cast<float>(y * Cell_Size));

			//�}�b�v�f�[�^����f�[�^�����o��

			//��
			if (rogue_like_dungeon.map_role[y][x].map_data == 1)
			{
				piece->SetWallNodeFlag(true);
			}

			//�G(�ۗ�)
			for (int i = 1; i < Mob_Max; i++)
			{
				if (rogue_like_dungeon.map_role[y][x].map_data == 3)
				{
					piece->SetEnemyNodeFlag(true);
				}
			}

			nodes.emplace_back(std::move(piece));

			advance.emplace_back(-1);
			node_id++;
		}
	}

	//edge�̏�����
	//distnation_node��- 1�ƂȂ�Aedge�͖���
	int distnation = 0;

	for (int y = 0; y < static_cast<int>(MapSize_Y); y++)
	{
		for (int x = 0; x < static_cast<int>(MapSize_X); x++)
		{
			int node = MapSize_Y * y + x;

			//edge�����
			std::shared_ptr<Edge> edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopCenter)];
			//{�֐���
			edge->Initialize(node, distnation);
			//��ԏ�̒i�łȂ����
			if (!(y <= 0))
			{
				distnation = node - MapSize_X;
			}
			edge->distnation_node = distnation;

			//edge�E�����
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopRight)];
			edge->Initialize(node, distnation);

			//��ԏ�̒i�ŉE�[�łȂ����
			if (!(y <= 0) && !(x >= MapSize_X - 1))
			{
				distnation = node - MapSize_X + 1;
			}
			edge->distnation_node = distnation;

			//edge->cost = 1.414f;

			//edge�E����
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterRight)];
			edge->Initialize(node, distnation);
			//�E�[�łȂ����
			if (!(x >= MapSize_X - 1))
			{
				distnation = node + 1;
			}
			edge->distnation_node = distnation;

			//edge�E������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomRight)];
			edge->Initialize(node, distnation);
			//��ԉ��̒i�ŉE�[�łȂ����
			if (!(y >= MapSize_Y - 1) && !(x >= MapSize_X - 1))
			{
				distnation = node + MapSize_X + 1;
			}
			edge->distnation_node = distnation;
			//edge->cost = 1.414f;

			//edge������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomCenter)];
			edge->Initialize(node, distnation);

			//��ԉ��̒i�łȂ����
			if (!(y >= MapSize_Y - 1))
			{
				distnation = node + MapSize_X;
			}
			edge->distnation_node = distnation;

			//edge��������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomLeft)];
			edge->Initialize(node, distnation);
			//��ԉ��̒i�ō��[�łȂ����
			if (!(y >= MapSize_Y - 1) && !(x <= 0))
			{
				distnation = node + MapSize_X - 1;
			}
			edge->distnation_node = distnation;
			//edge->cost = 1.414f;

			//edge������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterLeft)];
			edge->Initialize(node, distnation);
			//�E�[�łȂ����
			if (!(x <= 0))
			{
				distnation = node - 1;
			}
			edge->distnation_node = distnation;

			//edge�������
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopLeft)];
			edge->Initialize(node, distnation);
			//��ԏ�̒i�ō��[�łȂ����
			if (!(y <= 0) && !(x <= 0))
			{
				distnation = node - MapSize_X - 1;
			}
			edge->distnation_node = distnation;
			//edge->cost = 1.414f;

		}
	}
}

std::vector<int> HeuristicSearch::Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon)
{
	//�T�[�`���̔z��
	std::vector<std::shared_ptr<Edge>> candidate;

	Reset(rogue_like_dungeon);

	//������
	for (int i = 0; i < MapSize; i++)
	{
		nodes[i]->SetSearchedNodeFlag(false);
		nodes[i]->cost_from_start = 0.f;
		advance[i] = -1;
	}
	std::shared_ptr<Edge> now_edge = std::make_shared<Edge>();
	//�_�~�[
	now_edge->distnation_node = start_id;
	now_edge->origin_node = start_id;
	while (true)
	{
		float total_cost = 0;
		//�T���ς݂�edge�̕ۑ�
		advance[now_edge->distnation_node] = now_edge->origin_node;

		//����nowEdge��distnation_node���ړI�n�Ȃ�ړI�n�܂ł̍ŒZ�i�s���[�g��Ԃ�
		if (now_edge->distnation_node == goal_id)
		{
			return advance;
		}
		//nowEdge�̐ڑ���̃m�[�h���擾����B
		std::shared_ptr<Node>node = nodes[now_edge->distnation_node];

		for (int edge_num = 0; edge_num < EDGE_NUM; edge_num++)
		{
			std::shared_ptr<Edge> edge = node->edge[edge_num];
			if (edge->distnation_node >= 0)//����edge���L���Ȃ�
			{
				std::shared_ptr<Node> next_node = nodes[edge->distnation_node];

				// �X�^�[�g�n�_����̃R�X�g�ƃG�b�W�̃R�X�g��total_cost�ɉ��Z
				total_cost += (node->cost_from_start + edge->cost);

				//cost����//

				//----------

				//�R�X�g����x���v�Z����ĂȂ����A�V�����R�X�g���Ⴏ��ΒT�����ɂ���
				if (next_node->cost_from_start == 0.f || next_node->cost_from_start > total_cost)
				{
					if (!next_node->GetWallNodeFlag())//�T����₪�ǂȂ���ɓ���Ȃ�
					{
						candidate.emplace_back(edge);
					}
				}

				now_edge = SearchMinCost(candidate, now_edge, goal_id);
			}
		}
	}
	return advance;
}

std::shared_ptr<Edge> HeuristicSearch::SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> nowEdge, int goal_id)
{
	std::shared_ptr<Edge> shortest = nullptr;//�ŒZ�G�b�W���i�[�p
	int shortest_num = 0;//�ŒZ�G�b�W���ԍ��i�[�p
	float min_cost = FLT_MAX;

	for (int candidate_num = 0; candidate_num < static_cast<int>(candidate.size()); candidate_num++)
	{
		Edge* edge = candidate[candidate_num].get();
		//elemet�ɓ����Ă���edge�̃X�^�[�g�ʒu����̃R�X�g���v�Z����B
		float total_cost = 0;
		std::shared_ptr<Node> origin_node = nodes[edge->origin_node];
		std::shared_ptr<Node> distnation_node = nodes[edge->distnation_node];
		total_cost += (origin_node->cost_from_start + edge->cost);
		//cost����//

		//----------

		//�ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v��front_cost�Ɏ��o��(�܂��o�^����Ă��Ȃ��Ȃ�O�ƂȂ�)
		float front_cost = 0;
		front_cost = distnation_node->cost_from_start;

		//front_cost���܂��o�^����Ă��Ȃ����Afront_cost��菭�Ȃ��R�X�g���[�g��
		//�������ꂽ�Ȃ�A�ڑ���̃X�^�[�g�ʒu����̃R�X�g��total_cost�ɏ��������B
		if (front_cost == 0.f || front_cost >= total_cost)
		{
			std::shared_ptr<Node> goalNode = nodes[goal_id];//�S�[���̃m�[�h
			float AstarCost = heuristicCulc(distnation_node, goalNode);

			distnation_node->cost_from_start = total_cost;
			front_cost = total_cost+AstarCost;
		}

		//front_cost���A�����ׂĂ���G�b�W�̐ڑ���̃g�[�^���R�X�g�ȏ�̂��̒���
		//��ԏ������ڑ���̃X�^�[�g�ʒu����̃R�X�g�����G�b�W���ŒZ�Ƃ��ĕۑ��B
		std::shared_ptr<Node> now_node = nodes[nowEdge->distnation_node];
		if (min_cost > front_cost)
		{
			//candidate�ƌ��ݒT������egde�̔�r
			if (front_cost >= now_node->cost_from_start)
			{
				shortest = candidate[candidate_num];
				min_cost = front_cost;
				shortest_num = candidate_num;
			}
		}
	}
	return shortest;
}

//�Q�̃m�[�h�̕����������o���Ă��̋�����Ԃ�
float  HeuristicSearch::heuristicCulc(std::shared_ptr<Node> N1, std::shared_ptr<Node> N2)
{
	DirectX::XMFLOAT2 N1_pos = N1->GetNodePosition();
	DirectX::XMFLOAT2 N2_pos = N2->GetNodePosition();
	float y = N1_pos.y - N2_pos.y;
	float x = N1_pos.x - N2_pos.x;
	return  sqrtf((y * y) + (x * x)) / 20;
}
//**********************************************************
//
//		HeuristicSearch�N���X
//
//**********************************************************
#include "Engine/AI/HeuristicSearch.h"
#include "Engine/AI/Edge.h"
#include "Engine/AI/Node.h"
#include "Engine/AI/DungeonMake.h"

HeuristicSearch* HeuristicSearch::instance = nullptr;

HeuristicSearch::HeuristicSearch()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

HeuristicSearch::~HeuristicSearch()
{
	if (!advance.empty())
	{
		advance.clear();
	}


}

void HeuristicSearch::Reset(const RogueLikeDungeon& rogue_like_dungeon)
{
	// node�̏�����
	searched_edge.clear();
	int node_id = 0;
	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
	{
		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
		{
			auto piece = std::make_shared<Node>();
			piece->node_id = node_id;
			piece->SetNodePosition(static_cast<float>(x * CellSize), static_cast<float>(y * CellSize));

			// �}�b�v�f�[�^����f�[�^�����o��

			// ��
			if (rogue_like_dungeon.map_role[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				piece->SetWallNodeFlag(true);//�ǃt���O
			}

			// �G(�ۗ�)
			if (rogue_like_dungeon.map_role[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				piece->SetEnemyNodeFlag(true);//�G�t���O
			}

			nodes.emplace_back(std::move(piece));

			//�i�s���[�g�̏�����
			advance.emplace_back(-1);
			//advance_vector[y][x] = -1;
			node_id++;
		}
	}

	// edge�̏�����
	// destination_node��- 1�ƂȂ�Aedge�͖���
	int destination = 0;

	for (int y = 0; y < static_cast<int>(MapSize_Y); y++)
	{
		for (int x = 0; x < static_cast<int>(MapSize_X); x++)
		{
			const int node = y * MapSize_Y + x;

			// edge�����
			std::shared_ptr<Edge> edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopCenter)];
			//{�֐���
			edge->Initialize(node, destination);
			// ��ԏ�̒i�łȂ����
			if (!(y <= 0))
			{
				destination = node - MapSize_X;
			}
			edge->destination_node = destination;

			// edge�E�����
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopRight)];
			edge->Initialize(node, destination);

			// ��ԏ�̒i�ŉE�[�łȂ����
			if (!(y <= 0) && !(x >= MapSize_X - 1))
			{
				destination = node - MapSize_X + 1;
			}
			edge->destination_node = destination;

			//edge->cost = 1.414f;

			// edge�E����
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterRight)];
			edge->Initialize(node, destination);
			// �E�[�łȂ����
			if (!(x >= MapSize_X - 1))
			{
				destination = node + 1;
			}
			edge->destination_node = destination;

			// edge�E������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomRight)];
			edge->Initialize(node, destination);
			// ��ԉ��̒i�ŉE�[�łȂ����
			if (!(y >= MapSize_Y - 1) && !(x >= MapSize_X - 1))
			{
				destination = node + MapSize_X + 1;
			}
			edge->destination_node = destination;
			//edge->cost = 1.414f;

			// edge������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomCenter)];
			edge->Initialize(node, destination);

			// ��ԉ��̒i�łȂ����
			if (!(y >= MapSize_Y - 1))
			{
				destination = node + MapSize_X;
			}
			edge->destination_node = destination;

			// edge��������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomLeft)];
			edge->Initialize(node, destination);
			// ��ԉ��̒i�ō��[�łȂ����
			if (!(y >= MapSize_Y - 1) && !(x <= 0))
			{
				destination = node + MapSize_X - 1;
			}
			edge->destination_node = destination;
			//edge->cost = 1.414f;

			// edge������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterLeft)];
			edge->Initialize(node, destination);
			// �E�[�łȂ����
			if (!(x <= 0))
			{
				destination = node - 1;
			}
			edge->destination_node = destination;

			// edge�������
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopLeft)];
			edge->Initialize(node, destination);
			// ��ԏ�̒i�ō��[�łȂ����
			if (!(y <= 0) && !(x <= 0))
			{
				destination = node - MapSize_X - 1;
			}
			edge->destination_node = destination;
			//edge->cost = 1.414f;

		}
	}
}

std::vector<int> HeuristicSearch::Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon)
{
	// �T�[�`���̔z��
	std::vector<std::shared_ptr<Edge>> candidate;

	Reset(rogue_like_dungeon);

	// ������
	for (int i = 0; i < MapSize; i++)
	{
		nodes[i]->SetSearchedNodeFlag(false);
		nodes[i]->cost_from_start = 0.f;
		advance[i] = -1;
	}

	std::shared_ptr<Edge> now_edge = std::make_shared<Edge>();

	// �_�~�[
	now_edge->destination_node = start_id;
	now_edge->origin_node = start_id;

	while (true)
	{
		float total_cost = 0;
		// �T���ς݂�edge�̕ۑ�
		advance[now_edge->destination_node] = now_edge->origin_node;

		// ����nowEdge��distnation_node���ړI�n�Ȃ�ړI�n�܂ł̍ŒZ�i�s���[�g��Ԃ�
		if (now_edge->destination_node == goal_id)
		{
			return advance;
		}
		// nowEdge�̐ڑ���̃m�[�h���擾����B
		std::shared_ptr<Node>node = nodes[now_edge->destination_node];

		for (int edge_num = 0; edge_num < EDGE_NUM; edge_num++)
		{
			std::shared_ptr<Edge> edge = node->edge[edge_num];
			if (edge->destination_node >= 0)// ����edge���L���Ȃ�
			{
				std::shared_ptr<Node> next_node = nodes[edge->destination_node];

				// �X�^�[�g�n�_����̃R�X�g�ƃG�b�W�̃R�X�g��total_cost�ɉ��Z
				total_cost += (node->cost_from_start + edge->cost);

				// cost����
				//�G
				if (next_node->GetEnemyNodeFlag() == true)
				{
					total_cost += 5.0f;
				}
				//��
				if (next_node->GetWallNodeFlag() == true)
				{
					total_cost += 10.0f;
				}
				//----------

				// �R�X�g����x���v�Z����ĂȂ����A�V�����R�X�g���Ⴏ��ΒT�����ɂ���
				if (next_node->cost_from_start == 0.f || next_node->cost_from_start > total_cost)
				{
					if (!next_node->GetWallNodeFlag())//�T����₪�ǂȂ���ɓ���Ȃ�
					{
						candidate.emplace_back(edge);
					}
				}
				//�ŒZ�o�H���Z�o
				now_edge = SearchMinCost(candidate, now_edge, goal_id);
			}
		}
	}
}

std::shared_ptr<Edge> HeuristicSearch::SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> now_edge, int goal_id)
{
	std::shared_ptr<Edge> shortest = nullptr;// �ŒZ�G�b�W���i�[�p
	int shortest_num = 0;// �ŒZ�G�b�W���ԍ��i�[�p
	float min_cost = FLT_MAX;

	for (int candidate_num = 0; candidate_num < static_cast<int>(candidate.size()); candidate_num++)
	{
		Edge* edge = candidate[candidate_num].get();
		// edge�̃X�^�[�g�ʒu����̃R�X�g���v�Z����B
		float total_cost = 0;
		std::shared_ptr<Node> origin_node = nodes[edge->origin_node];
		std::shared_ptr<Node> distnation_node = nodes[edge->destination_node];
		total_cost += (origin_node->cost_from_start + edge->cost);
		// cost����

		//----------

		// �ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v��front_cost�Ɏ��o��(�܂��o�^����Ă��Ȃ��Ȃ�O�ƂȂ�)
		float front_cost = 0;
		front_cost = distnation_node->cost_from_start;

		// front_cost���܂��o�^����Ă��Ȃ����Afront_cost��菭�Ȃ��R�X�g���[�g��
		// �������ꂽ�Ȃ�A�ڑ���̃X�^�[�g�ʒu����̃R�X�g��total_cost�ɏ��������B
		if (front_cost == 0.f || front_cost >= total_cost)
		{
			std::shared_ptr<Node> goalNode = nodes[goal_id];//�S�[���̃m�[�h
			const float AstarCost = HeuristicCalculation(distnation_node, goalNode);

			distnation_node->cost_from_start = total_cost;
			front_cost = total_cost + AstarCost;
		}

		// front_cost���A�����ׂĂ���G�b�W�̐ڑ���̃g�[�^���R�X�g�ȏ�̂��̒���
		// ��ԏ������ڑ���̃X�^�[�g�ʒu����̃R�X�g�����G�b�W���ŒZ�Ƃ��ĕۑ��B
		std::shared_ptr<Node> now_node = nodes[now_edge->destination_node];
		if (min_cost > front_cost)
		{
			// candidate�ƌ��ݒT������egde�̔�r
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

// �Q�̃m�[�h�̕����������o���Ă��̋�����Ԃ�
float  HeuristicSearch::HeuristicCalculation(const std::shared_ptr<Node> n1, const std::shared_ptr<Node> n2)
{
	const DirectX::XMFLOAT2 N1_pos = n1->GetNodePosition();
	const DirectX::XMFLOAT2 N2_pos = n2->GetNodePosition();
	const float y = N1_pos.y - N2_pos.y;
	const float x = N1_pos.x - N2_pos.x;
	return  sqrtf((y * y) + (x * x)) / 20;
}

//Aster�̌��ʂ�2�����z��ɕϊ�����
//void HeuristicSearch::ConvertTwoDimensionalArray()
//{
//	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
//	{
//		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
//		{
//			//   x ��
//			// y		MapSize_Y*0----------------------MapSize_X*1-1
//			//��		MapSize_Y*1----------------------MapSize_X*2-1
//			//			MapSize_Y*2----------------------MapSize_X*3-1
//			//			MapSize_Y*3----------------------MapSize_X*4-1
//			//			MapSize_Y*4----------------------MapSize_X*5-1
//			//			MapSize_Y*5----------------------MapSize_X*6-1
//			//~~~~~~~~~~~~~~~~~
//			//			MapSize_Y*  ----------------------MapSize_Y*
//			//		  (MapSize_X - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//�i�s���[�g��񎟌��z��Ɋi�[
//			advance_vector[y][x] = advance[array_num];
//		}
//	}
//}
//�}�b�v����1�����z��ɕϊ�����
//void HeuristicSearch::ConvertOneDimensionalArray(const RogueLikeDungeon& rogue_like_dungeon)
//{
//	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
//	{
//		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
//		{
//			//   x ��
//			// y		MapSize_Y*0----------------------MapSize_X*1-1
//			//��		MapSize_Y*1----------------------MapSize_X*2-1
//			//			MapSize_Y*2----------------------MapSize_X*3-1
//			//			MapSize_Y*3----------------------MapSize_X*4-1
//			//			MapSize_Y*4----------------------MapSize_X*5-1
//			//			MapSize_Y*5----------------------MapSize_X*6-1
//			//~~~~~~~~~~~~~~~~~
//			//			MapSize_Y*  ----------------------MapSize_Y*
//			//		(MapSize_X - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//�}�b�v�����ꎟ���z��Ɋi�[
//			map_info[array_num] = static_cast<int>(rogue_like_dungeon.map_role[y][x].map_data);
//		}
//	}
//}

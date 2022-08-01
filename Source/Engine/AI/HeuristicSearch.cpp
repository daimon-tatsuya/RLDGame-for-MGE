//**********************************************************
//
//		HeuristicSearch�N���X
//
//**********************************************************

// �w�b�_�[
#include "Engine/AI/HeuristicSearch.h"
#include "Engine/AI/Edge.h"
#include "Engine/AI/Node.h"
#include "Engine/AI/DungeonMake.h"

// �C���X�^���X
HeuristicSearch* HeuristicSearch::instance = nullptr;

// �R���X�g���N�^
HeuristicSearch::HeuristicSearch()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

// �f�X�g���N�^
HeuristicSearch::~HeuristicSearch()
{
	// �R���e�i������Ȃ�
	if (!advance.empty())
	{
		advance.clear();
	}
}

//�m�[�h�̐ݒ�
void HeuristicSearch::NodesInitialize()
{
	// node�̏�����
	searched_edge.clear();
	int node_id = 0;
	for (int y = 0; y < MAP_SIZE_Y - 1; y++)
	{
		for (int x = 0; x < MAP_SIZE_X - 1; x++)
		{
			auto piece = std::make_shared<Node>();
			piece->node_id = node_id;
			piece->SetNodePosition(static_cast<float>(x), static_cast<float>(y));

			// �}�b�v�f�[�^����f�[�^�����o��
			RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

			//�}�b�v�f�[�^�̓G��ǂ��m�[�h�ɋL��

			// ��
			if (rogue_like_dungeon.GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				piece->SetIsWallNode(true);// �ǃt���O
			}

			// �G(�ۗ�)
			if (rogue_like_dungeon.GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				piece->SetIsEnemyNode(true);// �G�t���O
			}

			nodes.emplace_back(std::move(piece));

			node_id++;
		}
	}

	// edge�̏�����
	// destination_node��- 1�ƂȂ�Aedge�͖���
	int destination = 0;

	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			const int node = y * MAP_SIZE_Y + x;
			// {�֐���

			// edge�����
			std::shared_ptr<Edge> edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopCenter)];

			edge->Initialize(node, destination);
			// ��ԏ�̒i�łȂ����
			if (!(y <= 0))
			{
				destination = node - MAP_SIZE_X;
			}
			edge->destination_node_id = destination;

			// edge�E�����
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopRight)];
			edge->Initialize(node, destination);

			// ��ԏ�̒i�ŉE�[�łȂ����
			if (!(y <= 0) && !(x >= MAP_SIZE_X - 1))
			{
				destination = node - MAP_SIZE_X + 1;
			}
			edge->destination_node_id = destination;
			edge->cost = OBLIQUE_COST;

			

			// edge�E����
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterRight)];
			edge->Initialize(node, destination);
			// �E�[�łȂ����
			if (!(x >= MAP_SIZE_X - 1))
			{
				destination = node + 1;
			}
			edge->destination_node_id = destination;

			// edge�E������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomRight)];
			edge->Initialize(node, destination);
			// ��ԉ��̒i�ŉE�[�łȂ����
			if (!(y >= MAP_SIZE_Y - 1) && !(x >= MAP_SIZE_X - 1))
			{
				destination = node + MAP_SIZE_X + 1;
			}
			edge->destination_node_id = destination;
			// edge->cost = 1.414f;

			// edge������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomCenter)];
			edge->Initialize(node, destination);

			// ��ԉ��̒i�łȂ����
			if (!(y >= MAP_SIZE_Y - 1))
			{
				destination = node + MAP_SIZE_X;
			}
			edge->destination_node_id = destination;

			// edge��������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomLeft)];
			edge->Initialize(node, destination);
			// ��ԉ��̒i�ō��[�łȂ����
			if (!(y >= MAP_SIZE_Y - 1) && !(x <= 0))
			{
				destination = node + MAP_SIZE_X - 1;
			}
			edge->destination_node_id = destination;
			// edge->cost = 1.414f;

			// edge������
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterLeft)];
			edge->Initialize(node, destination);
			// �E�[�łȂ����
			if (!(x <= 0))
			{
				destination = node - 1;
			}
			edge->destination_node_id = destination;

			// edge�������
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopLeft)];
			edge->Initialize(node, destination);
			// ��ԏ�̒i�ō��[�łȂ����
			if (!(y <= 0) && !(x <= 0))
			{
				destination = node - MAP_SIZE_X - 1;
			}
			edge->destination_node_id = destination;
			// edge->cost = 1.414f;
		}
	}
}

//start_id����goal_id�̃R�X�g��}�����ŒZ�o�H���v�Z
bool HeuristicSearch::Search(int start_id, int goal_id)
{
	// �T�[�`���̔z��
	std::vector<std::shared_ptr<Edge>> candidate;

	NodesInitialize();

	// ������
	for (int i = 0; i < MAP_SIZE; i++)
	{
		nodes[i]->is_searched_node = false;
		nodes[i]->cost_from_start = 0.f;
	}

	// �i�s���[�g�̏�����
	advance.clear();

	// ���ݒT������edge���쐬
	std::shared_ptr<Edge> now_edge = std::make_shared<Edge>();

	// �X�^�[�g
	now_edge->destination_node_id = start_id;
	now_edge->origin_node_id = start_id;

	while (true)
	{
		float total_cost = 0;
		// �T���ς݂�edge�̕ۑ�
		// advance.push_back(now_edge->origin_node_id);

		advance[now_edge->destination_node_id] = now_edge->origin_node_id;

		// ����nowEdge��destination_node���ړI�n�Ȃ�ړI�n�܂ł̍ŒZ�i�s���[�g��Ԃ�
		if (now_edge->destination_node_id == goal_id)
		{

			advance.push_back(-1);
			return true;
		}
		// now_edge�̐ڑ���̃m�[�h���擾����B
		std::shared_ptr<Node>node = nodes[now_edge->destination_node_id];

		for (int edge_num = 0; edge_num < EDGE_NUM; edge_num++)// 8�������ׂ�
		{
			std::shared_ptr<Edge> edge = node->edge[edge_num];

			if (edge->destination_node_id >= 0)// ����edge���L���Ȃ�
			{
				const std::shared_ptr<Node> next_node = nodes[edge->destination_node_id];

				// �X�^�[�g�n�_����̃R�X�g�ƃG�b�W�̃R�X�g��total_cost�ɉ��Z
				total_cost += (node->cost_from_start + edge->cost);

				// cost����
				// �G
				if (next_node->GetIsEnemyNode() == true)
				{
					total_cost += 10.0f;
				}
				// ��
				if (next_node->GetIsWallNode() == true)
				{
					total_cost += 10.0f;
				}
				//----------

				// �R�X�g����x���v�Z����ĂȂ����A�V�����R�X�g���Ⴏ��ΒT�����ɂ���
				if (next_node->cost_from_start == 0.f || next_node->cost_from_start > total_cost)
				{
					if (!next_node->GetIsWallNode())// �T����₪�ǂȂ���ɓ���Ȃ�
					{
						candidate.push_back(edge);
					}
				}
				// �ŒZ�o�H���Z�o
				now_edge = SearchMinCost(candidate, now_edge, goal_id);

				if (now_edge == nullptr) { return false; }
			}
		}
	}
	return false;
}



// �Q�̃m�[�h�̕����������o���Ă��̋�����Ԃ�
std::shared_ptr<Edge> HeuristicSearch::SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> now_edge, int goal_id)
{
	std::shared_ptr<Edge> shortest = nullptr;// �ŒZ�G�b�W���i�[�p
	int shortest_num;// �ŒZ�G�b�W���ԍ��i�[�p
	float min_cost = FLT_MAX;

	for (int candidate_num = 0; candidate_num < static_cast<int>(candidate.size()); candidate_num++)
	{
		Edge* edge = candidate[candidate_num].get();
		// edge�̃X�^�[�g�ʒu����̃R�X�g���v�Z����B
		float total_cost = 0;
		const std::shared_ptr<Node> origin_node = nodes[edge->origin_node_id];
		const std::shared_ptr<Node> destination_node = nodes[edge->destination_node_id];
		total_cost += (origin_node->cost_from_start + edge->cost);
		// cost����

		//----------

		// �ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v��front_cost�Ɏ��o��(�܂��o�^����Ă��Ȃ��Ȃ�O�ƂȂ�)
		float front_cost = 0;
		front_cost = destination_node->cost_from_start;

		// front_cost���܂��o�^����Ă��Ȃ����Afront_cost��菭�Ȃ��R�X�g���[�g��
		// �������ꂽ�Ȃ�A�ڑ���̃X�^�[�g�ʒu����̃R�X�g��total_cost�ɏ��������B
		if (front_cost == 0.f || front_cost >= total_cost)
		{
			const std::shared_ptr<Node> goal_node = nodes[goal_id];//�S�[���̃m�[�h
			const float astar_cost = HeuristicCalculation(destination_node, goal_node);

			destination_node->cost_from_start = total_cost;
			front_cost = total_cost + astar_cost;
		}

		// front_cost���A�����ׂĂ���G�b�W�̐ڑ���̃g�[�^���R�X�g�ȏ�̂��̒���
		// ��ԏ������ڑ���̃X�^�[�g�ʒu����̃R�X�g�����G�b�W���ŒZ�Ƃ��ĕۑ��B
		const std::shared_ptr<Node> now_node = nodes[now_edge->destination_node_id];
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

// �ꎟ���z����}�b�v���ɕϊ�
DirectX::XMINT2 HeuristicSearch::ConvertMapPosition(int one_dimensional)
{
	return {one_dimensional / MAP_SIZE_Y, one_dimensional - (one_dimensional / MAP_SIZE_Y)};
}

// ���[���h���W(int x, int z)��1�����ɕϊ�����
int HeuristicSearch::ConvertWorldToOneDimensional(DirectX::XMFLOAT2 world)
{
	return static_cast<int>(world.y) * MAP_SIZE_Y + static_cast<int>(world.x);
}

int HeuristicSearch::ConvertWorldToOneDimensional(DirectX::XMINT2 world)
{
	return (world.y) * MAP_SIZE_Y + (world.x);
}

// �}�b�v����1�����z��ɕϊ�����
void HeuristicSearch::ConvertMapInformationToOneDimensionalArray()
{
	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	// ��
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			//   x ��
			// y		MAP_SIZE_Y*0----------------------MAP_SIZE_X*1-1
			//��		MAP_SIZE_Y*1----------------------MAP_SIZE_X*2-1
			//			MAP_SIZE_Y*2----------------------MAP_SIZE_X*3-1
			//			MAP_SIZE_Y*3----------------------MAP_SIZE_X*4-1
			//			MAP_SIZE_Y*4----------------------MAP_SIZE_X*5-1
			//			MAP_SIZE_Y*5----------------------MAP_SIZE_X*6-1
			// ~~~~~~~~~~~~~~~~~
			//			MAP_SIZE_Y* x----------------------MAP_SIZE_X*x-1
			//		(MAP_SIZE_Y - 1)							MAP_SIZE_X - 1
			const int array_num = y * MAP_SIZE_Y + x;
			// �}�b�v�����ꎟ���z��Ɋi�[
			map_info[array_num] = static_cast<int>(rogue_like_dungeon.GetMapRole()[y][x].map_data);
		}
	}
}
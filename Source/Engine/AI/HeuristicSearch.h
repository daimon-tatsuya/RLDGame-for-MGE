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
	DirectX::XMFLOAT3 destination_pos{};//�ړI�n�̃}�b�v��̈ʒu
	std::vector<int> path_information{};//�ŒZ�o�H
	unsigned int path_index{};//�ŒZ�o�H�p�̔z��index�ԍ�
	unsigned int path_size;//�ŒZ�o�H�̃R���e�i�̃T�C�Y�Apath_index-1�Ɣ�r���ē����Ȃ�
	//�����o�̏�����
	void Clear()
	{
		destination_pos = DirectX::XMFLOAT3(0, 0,0);
		path_information.clear();
		path_index = 0;
		path_size = 0;
	}

	//�ŒZ�o�H�̃T�C�Y��ݒ�
	void SetPathSize() { path_size = static_cast<unsigned int>(path_information.size()); }

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
	static float HeuristicCalculation(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

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
	/// <returns>�ŒZ�o�H�̈ʒu</returns>
	std::vector<int> Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon);

	///<summary>
	///2�����z��ɕϊ�����
	///</summary>


};
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
//			//			MapSize_Y*  ----------------------MapSize_X*x-1
//			//		  (MapSize_Y - 1)							MapSize_X - 1
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
//			//			MapSize_Y* x----------------------MapSize_X*x-1
//			//		(MapSize_Y - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//�}�b�v�����ꎟ���z��Ɋi�[
//			map_info[array_num] = static_cast<int>(rogue_like_dungeon.map_role[y][x].map_data);
//		}
//	}
//}
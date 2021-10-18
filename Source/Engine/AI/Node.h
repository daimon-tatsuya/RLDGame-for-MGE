#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "Engine/AI/Edge.h"

const int EDGE_NUM = 8;

class Node
{
private:
public:

	Node();
	~Node();

	/// <summary>
	/// start_node_flag�̐ݒ�
	/// HeuristicSearch�Ŏg�p
	/// true��node����HeuristicSearch���s��
	/// </summary>
	/// <param name="set"></param>
	void SetStartNodeFlag(bool set) { start_node_flag = set; }

	/// <summary>
	/// HeuristicSearch�Ŏg�p
	/// true��node����HeuristicSearch���s��
	/// </summary>
	/// <returns>start_node_flag</returns>
	bool GetStartNodeFlag() { return start_node_flag; }

	/// <summary>
	/// goal_node_flag�̐ݒ�
	/// HeuristicSearch�Ŏg�p
	/// true��node��ړI�n�Ƃ���HeuristicSearch���s��
	/// </summary>
	/// <param name="set"></param>
	void SetGoalNodeFlag(bool set) { goal_node_flag = set; }

	/// <summary>
	/// HeuristicSearch�Ŏg�p
	/// true��node��ړI�n�Ƃ���HeuristicSearch���s��
	/// </summary>
	/// <returns>goal_node_flag</returns>
	bool GetGoalNodeFlag() { return goal_node_flag; }

	/// <summary>
	/// player_node_flag�̐ݒ�
	/// true�Ȃ炱��node�̓v���C���[������
	/// </summary>
	/// <param name="set"></param>
	void SetPlayerNodeFlag(bool set) { player_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�̓v���C���[������
	/// </summary>
	/// <returns>player_node_flag</returns>
	bool GetPlayerNodeFlag() { return player_node_flag; }

	/// <summary>
	/// enemy_node_flag�̐ݒ�
	/// true�Ȃ炱��node�͓G������
	/// </summary>
	/// <param name="set"></param>
	void SetEnemyNodeFlag(bool set) { enemy_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�͓G������
	/// </summary>
	/// <returns>enemy_node_flag</returns>
	bool GetEnemyNodeFlag() { return enemy_node_flag; }

	/// <summary>
	/// wall_node_flag�̐ݒ�
	/// true�Ȃ炱��node�͕�
	/// </summary>
	/// <param name="set"></param>
	void SetWallNodeFlag(bool set) { wall_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�͕�
	/// </summary>
	/// <returns>wall_node_flag</returns>
	bool GetWallNodeFlag() { return wall_node_flag; }

	/// <summary>
	/// item_node_flag�̐ݒ�
	/// true�Ȃ炱��node�ɂ̓A�C�e�����u����Ă���
	/// </summary>
	/// <param name="set"></param>
	void SetItemNodeFlag(bool set) { item_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�ɂ̓A�C�e�����u����Ă���
	/// </summary>
	/// <returns>item_node_flag</returns>
	bool GetItemNodeFlag() { return item_node_flag; }

	/// <summary>
	/// searched_node_flag�̐ݒ�
	/// true�Ȃ炱��node�͒T���ς�
	/// </summary>
	/// <param name="set"></param>
	void SetSearchedNodeFlag(bool set) { searched_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�͒T���ς�
	/// </summary>
	/// <returns>searched_node_flag</returns>
	bool GetSearchedNodeFlag() { return searched_node_flag; }

	/// <summary>
	/// node��position�̐ݒ�
	/// </summary>
	/// <param name="pos">DirectX::XMFLOAT2</param>
	void SetNodePosition(DirectX::XMFLOAT2 pos) { position = pos; }

	/// <summary>
	/// node��position�̐ݒ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void SetNodePosition(float x, float y) { position = { x,y }; }

	/// <summary>
	///  node��position��x���W�ݒ�
	/// </summary>
	/// <param name="x"></param>
	void SetNodePositionX(float x) { position.x = x; }

	/// <summary>
	/// node��position��y���W�ݒ�
	/// </summary>
	/// <param name="y"></param>
	void SetNodePositionY(float y) { position.y = y; }

	/// <summary>
	///  node��position�̒l��Ԃ�
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT2 GetNodePosition() { return position; }

private:

	bool start_node_flag = false;//A*�n�߂�n�_
	bool goal_node_flag = false;//�ڕW�n�_
	bool player_node_flag = false;//Player�̂���n�_
	bool enemy_node_flag = false;//Enemy�̂���n�_
	bool wall_node_flag = false;//�ǂ̂���n�_
	bool item_node_flag = false;//�A�C�e���̂���n�_
	bool searched_node_flag = false;//�T���ς݂̒n�_

	DirectX::XMFLOAT2 position = { 0,0 };

public:
	int node_id = 0;
	std::vector<std::shared_ptr<Edge>> edge;//���̃m�[�h�����L����G�b�W���
	float cost = 1.0f;					//�m�[�h���̂̃R�X�g
	float cost_from_start = 100.f;//A*���n�߂��n�_����̍��v�R�X�g
};

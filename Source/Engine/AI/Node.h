#pragma once
//**********************************************************
//
//		Node�N���X
//
//**********************************************************

#include <vector>
#include <memory>
#include "Engine/Systems/Math.h"

//�O���錾
class Edge;

class Node final
{
private:

	bool is_start_node = false;		// A*�n�߂�n�_�Ȃ�true
	bool is_goal_node = false;		// �ڕW�n�_�Ȃ�true
	bool is_player_node = false;	// �v���C���[�̂���n�_�Ȃ�true
	bool is_enemy_node = false;	// �G�̂���n�_�Ȃ�true
	bool is_wall_node = false;		// �ǂ̂���n�_�Ȃ�true
	bool is_item_node = false;		// �A�C�e���̂���n�_�Ȃ�true

public:
	int node_id = 0;
	DirectX::XMFLOAT2 position{ 0,0 };
	std::vector<std::shared_ptr<Edge>> edge;// ���̃m�[�h�����L����G�b�W���
	float cost = 1.0f;		                             // �m�[�h���̂̃R�X�g
	float cost_from_start = 100.f;                 // A*���n�߂��n�_����̍��v�R�X�g
	bool is_searched_node = false;// �T���ς݂̒n�_�Ȃ�true
private:

public:

	Node();

	~Node() = default;

	/// <summary>
	/// <para>start_node_flag�̐ݒ�</para>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node����HeuristicSearch���s��</para>
	/// </summary>
	/// <param name="set_flag"></param>
	void SetIsStartNode(const bool set_flag) { is_start_node = set_flag; }

	/// <summary>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node����HeuristicSearch���s��</para>
	/// </summary>
	/// <returns>is_start_node</returns>
	bool GetIsStartNode() const { return is_start_node; }

	/// <summary>
	/// <para>goal_node_flag�̐ݒ�</para>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node��ړI�n�Ƃ���HeuristicSearch���s��</para>
	/// </summary>
	/// <param name="set_flag"></param>
	void SetIsGoalNode(bool set_flag) { is_goal_node = set_flag; }

	/// <summary>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node��ړI�n�Ƃ���HeuristicSearch���s��</para>
	/// </summary>
	/// <returns>is_goal_node</returns>
	bool GetIsGoalNode() const { return is_goal_node; }

	/// <summary>
	/// <para>player_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�̓v���C���[������</para>
	/// </summary>
	/// <param name="set_flag"></param>
	void SetIsPlayerNode(bool set_flag) { is_player_node = set_flag; }

	/// <summary>
	/// true�Ȃ炱��node�̓v���C���[������
	/// </summary>
	/// <returns>is_player_node</returns>
	bool GetIsPlayerNode() const { return is_player_node; }

	/// <summary>
	/// <para>enemy_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�͓G������</para>
	/// </summary>
	/// <param name="set_flag"></param>
	void SetIsEnemyNode(bool set_flag) { is_enemy_node = set_flag; }

	/// <summary>
	/// true�Ȃ炱��node�͓G������
	/// </summary>
	/// <returns>is_enemy_node</returns>
	bool GetIsEnemyNode() const { return is_enemy_node; }

	/// <summary>
	/// <para>wall_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�͕�</para>
	/// </summary>
	/// <param name="set_flag"></param>
	void SetIsWallNode(bool set_flag) { is_wall_node = set_flag; }

	/// <summary>
	/// true�Ȃ炱��node�͕�
	/// </summary>
	/// <returns>is_wall_node</returns>
	bool GetIsWallNode() const { return is_wall_node; }

	/// <summary>
	/// <para>item_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�ɂ̓A�C�e�����u����Ă���</para>
	/// </summary>
	/// <param name="set_flag"></param>
	void SetIsItemNode(bool set_flag) { is_item_node = set_flag; }

	/// <summary>
	/// true�Ȃ炱��node�ɂ̓A�C�e�����u����Ă���
	/// </summary>
	/// <returns>is_item_node</returns>
	bool GetIsItemNode() const { return is_item_node; }

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
	DirectX::XMFLOAT2 GetNodePosition() const { return position; }
};

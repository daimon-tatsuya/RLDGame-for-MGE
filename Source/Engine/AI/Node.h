#pragma once
//**********************************************************
//
//		Node�N���X
//
//**********************************************************

#include <d3d11.h>
#include <vector>
#include <memory>

#include "Engine/Systems/Math.h"
#include "Engine/AI/Edge.h"


class Node
{
private:
public:

	Node();
	~Node();

	/// <summary>
	/// <para>start_node_flag�̐ݒ�</para>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node����HeuristicSearch���s��</para>
	/// </summary>
	/// <param name="set"></param>
	void SetStartNodeFlag(bool set) { start_node_flag = set; }

	/// <summary>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node����HeuristicSearch���s��</para>
	/// </summary>
	/// <returns>start_node_flag</returns>
	bool GetStartNodeFlag() { return start_node_flag; }

	/// <summary>
	/// <para>goal_node_flag�̐ݒ�</para>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node��ړI�n�Ƃ���HeuristicSearch���s��</para>
	/// </summary>
	/// <param name="set"></param>
	void SetGoalNodeFlag(bool set) { goal_node_flag = set; }

	/// <summary>
	/// <para>HeuristicSearch�Ŏg�p</para>
	/// <para>true��node��ړI�n�Ƃ���HeuristicSearch���s��</para>
	/// </summary>
	/// <returns>goal_node_flag</returns>
	bool GetGoalNodeFlag() { return goal_node_flag; }

	/// <summary>
	/// <para>player_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�̓v���C���[������</para>
	/// </summary>
	/// <param name="set"></param>
	void SetPlayerNodeFlag(bool set) { player_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�̓v���C���[������
	/// </summary>
	/// <returns>player_node_flag</returns>
	bool GetPlayerNodeFlag() { return player_node_flag; }

	/// <summary>
	/// <para>enemy_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�͓G������</para>
	/// </summary>
	/// <param name="set"></param>
	void SetEnemyNodeFlag(bool set) { enemy_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�͓G������
	/// </summary>
	/// <returns>enemy_node_flag</returns>
	bool GetEnemyNodeFlag() { return enemy_node_flag; }

	/// <summary>
	/// <para>wall_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�͕�</para>
	/// </summary>
	/// <param name="set"></param>
	void SetWallNodeFlag(bool set) { wall_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�͕�
	/// </summary>
	/// <returns>wall_node_flag</returns>
	bool GetWallNodeFlag() { return wall_node_flag; }

	/// <summary>
	/// <para>item_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�ɂ̓A�C�e�����u����Ă���</para>
	/// </summary>
	/// <param name="set"></param>
	void SetItemNodeFlag(bool set) { item_node_flag = set; }

	/// <summary>
	/// true�Ȃ炱��node�ɂ̓A�C�e�����u����Ă���
	/// </summary>
	/// <returns>item_node_flag</returns>
	bool GetItemNodeFlag() { return item_node_flag; }

	/// <summary>
	/// <para>searched_node_flag�̐ݒ�</para>
	/// <para>true�Ȃ炱��node�͒T���ς�</para>
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

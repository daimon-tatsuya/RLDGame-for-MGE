#pragma once
//**********************************************************
//
//		Character�N���X
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/AI/HeuristicSearch.h"

class Character:
public Object
{
private:

	int	 current_health = 5;//���O��ς��邩�� health��life,HP
	int	 max_health = 5;

	bool				is_decide_pos = false;//�ʒu�����܂�������break���邽�߂Ɏg��

public:

	RogueLikeDungeon* stage_information = nullptr; // �}�b�v���
	ShortestPath* shortest_path{};//�ŒZ�o�H

private:

	/// <summary>
/// �L���X�e�[�g�}�V���̏�����
/// </summary>
	virtual void FiniteStateMachineInitialize() {}


public:
	// ���N��Ԃ��擾
	int GetCurrentHealth() const { return current_health; }

	// ���N��Ԃ�ݒ�
	void SetCurrentHealth(const int health) { this->current_health = health; }

	// �ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return max_health; }

	// �ő匒�N��Ԃ�ݒ�
	void SetMaxHealth() { this->max_health = max_health; }

	// �}�b�v�����擾
	RogueLikeDungeon* GetStageInformation() const { return stage_information; }

	//�ŒZ�o�H��ݒ�
	void SetShortestPath(std::vector<int> path) const { shortest_path->path_information = std::move(path); }

	bool GetIsDecidePos() const { return  is_decide_pos; }

	void SetIsDecidePos(bool set) { is_decide_pos = set; }
};

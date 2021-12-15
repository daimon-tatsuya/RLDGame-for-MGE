#pragma once
//*********************************************************
//
//		StateMachine�N���X
//
//*********************************************************



#include<crtdbg.h>
#include<functional>
#include<map>


//template���w�b�_�[�������ꍇ
//https://pknight.hatenablog.com/entry/20090826/1251303641
/// <summary>
///  �L���X�e�[�g�}�V���N���X
/// </summary>
/// <typeparam name="Key">�X�e�[�g��index</typeparam>
/// <typeparam name="ReturnValue">�X�e�[�g�̖߂�l</typeparam>
/// <typeparam name="...Args">�X�e�[�g�̊֐��̈���</typeparam>
template<typename Key, typename ReturnValue = void, typename... Args>
class StateMachine final
{
private:
	Key current_state{ static_cast<int>(-1) };
	Key old_state{ static_cast<int>(-1) };		// State�̏������ȂǂɎg��

	/// <summary>
	/// State���i�[����
	/// </summary>
	using State = std::map < Key, std::function<ReturnValue(Args...)>>;
	State state_pool;

	bool is_first_time = false;//�X�e�[�g�̐؂�ւ�

public:

private:

public:
	/// <summary>
	/// �X�e�[�g��ǉ�
	/// </summary>
	/// <param name="state">enum class �����^��State��</param>
	/// <param name="callback">�����_��</param>
	void AddState(Key state, std::function<ReturnValue(Args...)> callback)
	{
		state_pool.insert(std::make_pair(state, callback));
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	void Update(Args... args)
	{
		// current_state�̗v�f���܂܂�ĂȂ�
		_ASSERT_EXPR(state_pool.contains(current_state) == true, "Not found state");

		state_pool.at(current_state)(args...);

	}


	/// <summary>
	/// �X�e�[�g�ɓ��������A�ŏ��̎��s���𒲂ׂ�
	/// </summary>
	/// <returns></returns>
	bool IsStateFirstTime()
	{

		if (!is_first_time)
		{
			return false;
		}

		is_first_time = false;

		return true;
	}

	void SetState(Key state)
	{
		old_state = current_state;
		current_state = state;
		is_first_time = true;
	}

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	int GetState() { return static_cast<int>(current_state); }
	int GetOldState() { return static_cast<int>(old_state); }

};



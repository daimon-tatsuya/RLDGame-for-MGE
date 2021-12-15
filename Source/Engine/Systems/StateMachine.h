#pragma once
//*********************************************************
//
//		StateMachineクラス
//
//*********************************************************



#include<crtdbg.h>
#include<functional>
#include<map>


//templateをヘッダー書いた場合
//https://pknight.hatenablog.com/entry/20090826/1251303641
/// <summary>
///  有限ステートマシンクラス
/// </summary>
/// <typeparam name="Key">ステートのindex</typeparam>
/// <typeparam name="ReturnValue">ステートの戻り値</typeparam>
/// <typeparam name="...Args">ステートの関数の引数</typeparam>
template<typename Key, typename ReturnValue = void, typename... Args>
class StateMachine final
{
private:
	Key current_state{ static_cast<int>(-1) };
	Key old_state{ static_cast<int>(-1) };		// Stateの初期化などに使う

	/// <summary>
	/// Stateを格納する
	/// </summary>
	using State = std::map < Key, std::function<ReturnValue(Args...)>>;
	State state_pool;

	bool is_first_time = false;//ステートの切り替え

public:

private:

public:
	/// <summary>
	/// ステートを追加
	/// </summary>
	/// <param name="state">enum class 整数型のState名</param>
	/// <param name="callback">ラムダ式</param>
	void AddState(Key state, std::function<ReturnValue(Args...)> callback)
	{
		state_pool.insert(std::make_pair(state, callback));
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	void Update(Args... args)
	{
		// current_stateの要素が含まれてない
		_ASSERT_EXPR(state_pool.contains(current_state) == true, "Not found state");

		state_pool.at(current_state)(args...);

	}


	/// <summary>
	/// ステートに入った時、最初の実行かを調べる
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



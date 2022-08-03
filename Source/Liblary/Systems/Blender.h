#pragma once
//**********************************************************
//
//		Blender.h
//
//**********************************************************
#include <array>
#include <d3d11.h>
#include <wrl.h>

enum class  BlendState : int//ブレンドの種類
	{
	None,
	Alpha,
	Add,
	Subtract,
	Replace,
	Multiply,
	Lighten,
	Darken,
	Screen,

	End
	};
/// <summary>
/// BlendStateを作成するクラス
/// </summary>
class Blender final
{
private:
	BlendState current_state = { BlendState::None };//現在のブレンドステート

	std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>, static_cast<u_int>(BlendState::End)>
		blend_states = { nullptr };//ブレンドステートのコンテナ
public:

private:
public:

	///<summary>
	///コンストラクタ
	///< summary>
	/// <param name="device">デバイス</param>
	Blender(ID3D11Device* device);

	//デストラクタ
	~Blender()= default;

	///<summary>
	/// ブレンドステートをアクティブにする
	///< summary>
	/// <param name="device_context">デバイス</param>
	/// <param name="state">アクティブにするブレンドステート</param>
	void Activate(ID3D11DeviceContext* device_context, BlendState state);
};
#pragma once
//**********************************************************
//
//		Blender.h
//
//**********************************************************
#include <array>
#include <d3d11.h>
#include <wrl.h>

enum class  BlendState : int//�u�����h�̎��
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
/// BlendState���쐬����N���X
/// </summary>
class Blender final
{
private:
	BlendState current_state = { BlendState::None };//���݂̃u�����h�X�e�[�g

	std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>, static_cast<u_int>(BlendState::End)>
		blend_states = { nullptr };//�u�����h�X�e�[�g�̃R���e�i
public:

private:
public:

	///<summary>
	///�R���X�g���N�^
	///< summary>
	/// <param name="device">�f�o�C�X</param>
	Blender(ID3D11Device* device);

	//�f�X�g���N�^
	~Blender()= default;

	///<summary>
	/// �u�����h�X�e�[�g���A�N�e�B�u�ɂ���
	///< summary>
	/// <param name="device_context">�f�o�C�X</param>
	/// <param name="state">�A�N�e�B�u�ɂ���u�����h�X�e�[�g</param>
	void Activate(ID3D11DeviceContext* device_context, BlendState state);
};
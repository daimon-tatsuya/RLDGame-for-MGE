#pragma once
//**********************************************************
//
//		ShaderManager�N���X
//
//**********************************************************
#include <memory>
#include <string>
#include <map>
#include "Engine/Systems/Shader.h"

/// <summary>
/// Shader���Ǘ�����N���X
/// </summary>
class ShaderManager
{
private:

public:

	ShaderManager() {}
	~ShaderManager();
	enum class ShaderName : int
	{
		Lambert = 0,
		LambertWireFrame,
		NoTexture,

		End
	};

	void AddShader(ShaderName shader_name, std::shared_ptr<Shader>);
	// �V�F�[�_�[�̌Ăяo��
	std::shared_ptr<Shader>GetShader(ShaderName shader_name);
private:

	using ShaderMap = std::map<ShaderName, std::weak_ptr<Shader>>;

	ShaderMap		shaders;
public:
};

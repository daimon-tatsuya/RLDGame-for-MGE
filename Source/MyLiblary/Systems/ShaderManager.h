#pragma once
//**********************************************************
//
//		ShaderManager�N���X
//
//**********************************************************

#include <memory>
#include <map>

//�O���錾
class Shader;

/// <summary>
/// Shader���Ǘ�����N���X
/// </summary>
class ShaderManager final
{
public:
	enum class ShaderName : int
	{
		Lambert = 0,
		LambertWireFrame,
		NoTexture,

		End
	};

private:

	using ShaderMap = std::map<ShaderName, std::weak_ptr<Shader>>;

	ShaderMap		shaders;
private:

public:

	ShaderManager() = default;;
	~ShaderManager();

	void AddShader(ShaderName shader_name, std::shared_ptr<Shader>);

	// �V�F�[�_�[�̌Ăяo��
	std::shared_ptr<Shader>GetShader(ShaderName shader_name);
};

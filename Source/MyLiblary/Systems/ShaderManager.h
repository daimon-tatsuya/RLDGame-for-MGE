#pragma once
//**********************************************************
//
//		ShaderManagerクラス
//
//**********************************************************

#include <memory>
#include <map>

//前方宣言
class Shader;

/// <summary>
/// Shaderを管理するクラス
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

	// シェーダーの呼び出し
	std::shared_ptr<Shader>GetShader(ShaderName shader_name);
};

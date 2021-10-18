#pragma once
#include <memory>
#include <string>
#include <map>
#include "Engine/Systems/Shader.h"

class ShaderManager
{
private:

public:
	//// 唯一のインスタンス取得
	//static ShaderManager& Instance()
	//{
	//	static ShaderManager instance;
	//	return instance;
	//}
	ShaderManager() {}
	~ShaderManager() {}
	enum class ShaderName : int
	{
		Lambert = 0,
		LambertWireFrame,
		NoTexture,

		End
	};

	void AddShader(ShaderName shader_name, std::shared_ptr<Shader>);
	// シェーダーの呼び出し
	std::shared_ptr<Shader>GetShader(ShaderName shader_name);
private:

	using ShaderMap = std::map<ShaderName, std::weak_ptr<Shader>>;

	ShaderMap		shaders;
public:
};

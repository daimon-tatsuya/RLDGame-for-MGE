//**********************************************************
//
//		ShaderManagerクラス
//
//**********************************************************
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/Shader.h"

ShaderManager::~ShaderManager()
{
	shaders.clear();
}

void ShaderManager::AddShader(ShaderName shader_name, std::shared_ptr<Shader> shader)
{
	//モデル検索
	ShaderMap::iterator iterator = shaders.find(shader_name);
	if (iterator != shaders.end()) return;

	shaders[shader_name] = shader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(ShaderName shader_name)
{
	//モデル検索
	ShaderMap::iterator iterator = shaders.find(shader_name);
	if (iterator != shaders.end())
	{
		// 読み込み済みのモデルリソースを返す
		return iterator->second.lock();
	}

	return nullptr;
}
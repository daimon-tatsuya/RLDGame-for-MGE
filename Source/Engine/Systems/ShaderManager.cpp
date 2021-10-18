#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/ShaderManager.h"

void ShaderManager::AddShader(ShaderName shader_name, std::shared_ptr<Shader> shader)
{
	//モデル検索
	ShaderMap::iterator it = shaders.find(shader_name);
	if (it != shaders.end()) return;

	shaders[shader_name] = shader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(ShaderName shader_name)
{
	//モデル検索
	ShaderMap::iterator it = shaders.find(shader_name);
	if (it != shaders.end())
	{
		// 読み込み済みのモデルリソースを返す
		return it->second.lock();
	}

	return nullptr;
}
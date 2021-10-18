#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/ShaderManager.h"

void ShaderManager::AddShader(ShaderName shader_name, std::shared_ptr<Shader> shader)
{
	//���f������
	ShaderMap::iterator it = shaders.find(shader_name);
	if (it != shaders.end()) return;

	shaders[shader_name] = shader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(ShaderName shader_name)
{
	//���f������
	ShaderMap::iterator it = shaders.find(shader_name);
	if (it != shaders.end())
	{
		// �ǂݍ��ݍς݂̃��f�����\�[�X��Ԃ�
		return it->second.lock();
	}

	return nullptr;
}
//**********************************************************
//
//		ShaderManager�N���X
//
//**********************************************************
#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/ShaderManager.h"
#include "Liblary/Systems/Shader.h"

ShaderManager::~ShaderManager()
{
	shaders.clear();
}

void ShaderManager::AddShader(ShaderName shader_name, std::shared_ptr<Shader> shader)
{
	//���f������
	ShaderMap::iterator iterator = shaders.find(shader_name);
	if (iterator != shaders.end()) return;

	shaders[shader_name] = shader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(ShaderName shader_name)
{
	//���f������
	ShaderMap::iterator iterator = shaders.find(shader_name);
	if (iterator != shaders.end())
	{
		// �ǂݍ��ݍς݂̃��f�����\�[�X��Ԃ�
		return iterator->second.lock();
	}

	return nullptr;
}
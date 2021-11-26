#pragma once
//**********************************************************
//
//		ResourceManager�N���X
//
//**********************************************************
#include <memory>
#include <string>
#include <map>
#include "Engine/Objects/ModelResource.h"


/// <summary>
///	 2��ڂ̓ǂݍ��݂𐧌�����N���X
/// </summary>
class ResourceManager
{
private:
	ResourceManager() {}
	~ResourceManager();
public:
	// �B��̃C���X�^���X�擾
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	// ���f�����\�[�X�ǂݍ���
	std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

private:
	/// <summary>
	/// �ǂݍ��񂾃��f���f�[�^�̎Q�Ɛ���܂Ƃ߂�
	/// </summary>
	using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

	ModelMap		models;
public:
};

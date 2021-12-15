//**********************************************************
//
//		ResourceManager�N���X
//
//**********************************************************
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/ResourceManager.h"
#include "Engine/Objects/ModelResource.h"

ResourceManager::~ResourceManager()
{
	if(!models.empty())//�R���e�i�T�C�Y�� 0 �̂Ƃ��� true, �����łȂ��Ƃ��� false�B
	{
		models.clear();
	}
}

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	//���f������
	const ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		//�Q�Ɛ悪�������ĂȂ����m�F����
		if (!it->second.expired())
		{
			// �ǂݍ��ݍς݂̃��f�����\�[�X��Ԃ�
			return it->second.lock();
		}
	}
	// �V�K���f�����\�[�X�쐬���ǂݍ���
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<ModelResource>(device, filename);

	// �}�b�v�ɓo�^
	models[filename] = model;
	return model;
}
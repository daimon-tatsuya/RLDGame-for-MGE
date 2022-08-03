//**********************************************************
//
//		ResourceManager�N���X
//
//**********************************************************
#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/ResourceManager.h"
#include "Liblary/Objects/ModelResource.h"

ResourceManager::~ResourceManager()
{
	if (!models.empty())//�R���e�i�T�C�Y�� 0 �̂Ƃ��� true, �����łȂ��Ƃ��� false�B
	{
		models.clear();
	}
}

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	//���f������
	const ModelMap::iterator iterator = models.find(filename);
	if (iterator != models.end())
	{
		//�Q�Ɛ悪�������ĂȂ����m�F����
		if (!iterator->second.expired())
		{
			// �ǂݍ��ݍς݂̃��f�����\�[�X��Ԃ�
			return iterator->second.lock();
		}
	}
	// �V�K���f�����\�[�X�쐬���ǂݍ���
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<ModelResource>(device, filename);

	// �}�b�v�ɓo�^
	models[filename] = model;
	return model;
}
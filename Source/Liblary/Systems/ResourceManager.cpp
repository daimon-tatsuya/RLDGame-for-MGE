//**********************************************************
//
//		ResourceManagerクラス
//
//**********************************************************
#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/ResourceManager.h"
#include "Liblary/Objects/ModelResource.h"

ResourceManager::~ResourceManager()
{
	if (!models.empty())//コンテナサイズが 0 のときに true, そうでないときに false。
	{
		models.clear();
	}
}

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	//モデル検索
	const ModelMap::iterator iterator = models.find(filename);
	if (iterator != models.end())
	{
		//参照先が解放されてないか確認する
		if (!iterator->second.expired())
		{
			// 読み込み済みのモデルリソースを返す
			return iterator->second.lock();
		}
	}
	// 新規モデルリソース作成＆読み込み
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<ModelResource>(device, filename);

	// マップに登録
	models[filename] = model;
	return model;
}
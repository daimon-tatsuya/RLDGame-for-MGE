#include <WICTextureLoader11.h>
#include <DDSTextureLoader11.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>
#include <map>
#include <shlwapi.h>
#include <algorithm>

#include "Engine/Systems/misc.h"
#include "Engine/Systems/Texture.h"

HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** shaderResourceView, D3D11_TEXTURE2D_DESC* texture2dDesc)
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage image;

	static std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> cache;
	auto it = cache.find(filename);

	if (it != cache.end())
	{
		*shaderResourceView = it->second.Get();
		(*shaderResourceView)->AddRef();
		(*shaderResourceView)->GetResource(resource.GetAddressOf());
	}
	else
	{
		//テクスチャファイルの拡張子を検索して、
		//拡張子ごとの読み込みを行う
		std::wstring extension = PathFindExtensionW(filename);
		std::transform(extension.begin(), extension.end(), extension.begin(), ::towlower);
		if (L".png" == extension || L".jpeg" == extension || L".jpg" == extension || L".jpe" == extension || L".gif" == extension || L".tiff" == extension || L".tif" == extension || L".bmp" == extension)
		{
			hr = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &metadata, image);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
		}
		else if (L".dds" == extension)
		{
			hr = DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, &metadata, image);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
		}
		else if (L".tga" == extension || L".vda" == extension || L".icb" == extension || L".vst" == extension)
		{
			hr = DirectX::LoadFromTGAFile(filename, &metadata, image);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
		}
		//読み込めなかったらダミーテクスチャを貼る
		else
		{
			//	MakeDummyTexture(device, shaderResourceView);
			return  hr;
		}

		if (hr == S_OK)
		{
			//HRESULT DirectX::CreateShaderResourceViewEx(
			//	ID3D11Device* pDevice,
			//	const Image* srcImages,
			//	size_t nimages,
			//	const TexMetadata& metadata,
			//	D3D11_USAGE usage,
			//	unsigned int bindFlags,
			//	unsigned int cpuAccessFlags,
			//	unsigned int miscFlags,
			//	bool forceSRGB,
			//	ID3D11ShaderResourceView** ppSRV);
			hr = DirectX::CreateShaderResourceViewEx(
				device,
				image.GetImages(),
				image.GetImageCount(),
				metadata,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_SHADER_RESOURCE,
				0,
				D3D11_RESOURCE_MISC_TEXTURECUBE,
				false,
				shaderResourceView);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
		}
	}

	cache.insert(std::make_pair(filename, *shaderResourceView));

	(*shaderResourceView)->GetResource(resource.GetAddressOf());

	// テクスチャ情報の取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	texture2d->GetDesc(texture2dDesc);

	return hr;
}

//e.g.
//referrer_filename <= L"data/bison.obj"
//referent_filename <= L"/user/textures/bison.png"
//combined_resource_path = > L"/data/bison.png"

HRESULT MakeDummyTexture(ID3D11Device* device, ID3D11ShaderResourceView** shaderResourceView)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texture2d_desc = {};
	texture2d_desc.Width = 1;
	texture2d_desc.Height = 1;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	u_int color = 0xFFFFFFFF;
	subresource_data.pSysMem = &color;
	subresource_data.SysMemPitch = 4;
	subresource_data.SysMemSlicePitch = 4;

	ID3D11Texture2D* texture2d;
	hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(texture2d, &shader_resource_view_desc, shaderResourceView);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

	texture2d->Release();

	return hr;
}
void CombineResourcePath(wchar_t(&combinedResourcePath)[256], const wchar_t* referrerFilename, const wchar_t* referentFilename)
{
	const wchar_t delimiters[] = { L'\\', L'/' };
	// extract directory from obj_filename
	wchar_t directory[256] = {};
	for (wchar_t delimiter : delimiters)
	{
		wchar_t* p = wcsrchr(const_cast<wchar_t*>(referrerFilename), delimiter);
		if (p)
		{
			memcpy_s(directory, 256, referrerFilename, (p - referrerFilename + 1) * sizeof(wchar_t));
			break;
		}
	}
	// extract filename from resource_filename
	wchar_t filename[256];
	wcscpy_s(filename, referentFilename);
	for (wchar_t delimiter : delimiters)
	{
		wchar_t* p = wcsrchr(filename, delimiter);
		if (p)
		{
			wcscpy_s(filename, p + 1);
			break;
		}
	}
	// combine directory and filename
	wcscpy_s(combinedResourcePath, directory);
	wcscat_s(combinedResourcePath, filename);
};
#pragma once
//**********************************************************
//
//		Texture
//
//**********************************************************

#include <d3d11.h>

// Textureクラス ShaderResouceViewを持つ　ディファード用

HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename, ID3D11ShaderResourceView** shaderResourceView, D3D11_TEXTURE2D_DESC* texture2dDesc);
HRESULT MakeDummyTexture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view);
void CombineResourcePath(wchar_t(&combinedResourcePath)[256], const wchar_t* referrerFilename, const wchar_t* referentFilename);



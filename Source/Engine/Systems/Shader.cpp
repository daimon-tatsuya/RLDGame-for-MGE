//**********************************************************
//
//		ShaderƒNƒ‰ƒX
//
//**********************************************************
#include<memory>
#include <string>
#include<map>
#include <wrl.h>
#include "Engine/Systems/misc.h"
#include "Engine/Systems/Shader.h"

HRESULT CreateVertexShaderFromCso(ID3D11Device* device, const char* csoName, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements)
{
	struct set_of_vertex_shader_and_input_layout
	{
		set_of_vertex_shader_and_input_layout(ID3D11VertexShader* vertex_shader, ID3D11InputLayout* input_layout) : vertex_shader(vertex_shader), input_layout(input_layout) {}
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	};
	static std::map<std::string, set_of_vertex_shader_and_input_layout> cache;

	auto it = cache.find(csoName);
	if (it != cache.end())
	{
		*vertexShader = it->second.vertex_shader.Get();
		(*vertexShader)->AddRef();
		*inputLayout = it->second.input_layout.Get();
		(*inputLayout)->AddRef();
		return S_OK;
	}
	FILE* fp = nullptr;
	fopen_s(&fp, csoName, "rb");
	_ASSERT_EXPR_A(fp, "CSO File not found");

	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::unique_ptr<unsigned char[]> cso_data = std::make_unique<unsigned char[]>(cso_sz);
	fread(cso_data.get(), cso_sz, 1, fp);
	fclose(fp);

	HRESULT hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertexShader);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

	hr = device->CreateInputLayout(inputElementDesc, numElements, cso_data.get(), cso_sz, inputLayout);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

	cache.insert(std::make_pair(csoName, set_of_vertex_shader_and_input_layout(*vertexShader, *inputLayout)));

	return hr;
}

HRESULT CreatePixelShaderFromCso(ID3D11Device* device, const char* csoName, ID3D11PixelShader** pixelShader)
{
	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> cache;
	auto it = cache.find(csoName);
	if (it != cache.end())
	{
		//it->second.Attach(*pixel_shader);
		*pixelShader = it->second.Get();
		(*pixelShader)->AddRef();
		return S_OK;
	}

	// UNIT.02
	FILE* fp = nullptr;
	fopen_s(&fp, csoName, "rb");
	_ASSERT_EXPR_A(fp, "CSO File not found");

	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::unique_ptr<unsigned char[]> cso_data = std::make_unique<unsigned char[]>(cso_sz);
	fread(cso_data.get(), cso_sz, 1, fp);
	fclose(fp);

	HRESULT hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixelShader);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

	cache.insert(std::make_pair(csoName, *pixelShader));

	return hr;
}
#include <fstream>
#include <vector>

#include "Engine/Systems/misc.h"
#include "Engine/Objects/StaticMesh.h"

#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Texture.h"

StaticMesh::StaticMesh(ID3D11Device* device, const wchar_t* objFilename, bool flippingVertexCoordinates/*UNIT.13*/)
{
	std::vector<Vertex> vertices;
	std::vector<u_int> indices;
	u_int current_index = 0;

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT2> texcoords;
	std::vector<DirectX::XMFLOAT3> normals;

	std::vector<std::wstring> mtl_filenames;

	// parse OBJ file
	{
		std::wifstream fin(objFilename);
		_ASSERT_EXPR(fin, L"'OBJ file not fuund.");
		wchar_t command[256];
		while (fin)
		{
			fin >> command;
			if (0 == wcscmp(command, L"v"))
			{
				// v x y z w
				//
				//幾何学的な頂点とそのx y z座標を指定します。
				//曲線とサーフェスには4番目の同次座標も必要です
				//ウェイトを呼び出しました。
				//
				// x y zは頂点のx、y、z座標です。これらは
				//頂点の位置を定義する浮動小数点数
				// 3つの次元。
				//
				// wは有理曲線と曲面に必要な重みです。
				// 非有理曲線と非曲面には必要ありません。
				// wの値を指定します。デフォルトは1.0です。
				float x, y, z;
				fin >> x >> y >> z;
				positions.push_back(DirectX::XMFLOAT3(x, y, z));
				fin.ignore(1024, L'\n');
			}

			else if (0 == wcscmp(command, L"vt"))
			{
				// vt u v w
				//
				//テクスチャ頂点とその座標を指定します。1Dテクスチャ
				//テクスチャ座標はuのみ必要、2Dテクスチャはuの両方が必要
				//およびvテクスチャ座標、および3Dテクスチャには3つすべてが必要
				//座標。
				//
				// uはテクスチャの水平方向の値です。
				//
				// vはオプションの引数です。
				//
				// vはテクスチャの垂直方向の値です。
				//デフォルトは0です。
				//
				// wはオプションの引数です。
				//
				// wはテクスチャの深さの値です。デフォルトは0です。
				float u, v;
				fin >> u >> v;

				texcoords.push_back(DirectX::XMFLOAT2(u, flippingVertexCoordinates ? 1.0f - v : v));
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"vn"))
			{
				// vn i j k
				//
				//コンポーネントi、j、kを持つ法線ベクトルを指定します。
				//
				//頂点法線はスムーズに影響します-ジオメトリのシェーディングとレンダリング.0
				//ポリゴンの場合、実際のファセットの代わりに頂点法線が使用されます
				//法線。サーフェスの場合、頂点法線は
				//サーフェス全体と実際の解析サーフェス法線を置き換えます。
				//
				//頂点法線が存在する場合、スムージンググループに優先します。
				//
				// i j kは、頂点法線のi、j、k座標です。
				//それらは浮動小数点数です。
				FLOAT i, j, k;
				fin >> i >> j >> k;
				normals.push_back(DirectX::XMFLOAT3(i, j, k));
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"f"))
			{
				// f v1 / vt1 / vn1 v2 / vt2 / vn2 v3 / vt3 / vn3 。 。
				//
				//オプションでテクスチャ頂点と頂点法線参照を含める
				//数値。
				//
				//頂点、テクスチャ頂点、および
				//頂点法線はスラッシュ（/）で区切る必要があります。スペースはありません
				//数値とスラッシュの間。
				//
				// vは面要素の頂点の参照番号です。A
				//少なくとも3つの頂点が必要です。
				//
				// vtはオプションの引数です。
				//
				// vtは面のテクスチャ頂点の参照番号
				//要素。常に最初のスラッシュの後に続きます。
				//
				// vnはオプションの引数です。
				//
				// vnは、face要素の頂点法線の参照番号です。
				//常に2番目のスラッシュの後に続く必要があります。
				//
				//面要素は表面法線を使用して向きを示します。
				//頂点は面を中心に反時計回りに並べられます。
				//面と法線はビューアの方向を指します。頂点の場合
				//順序は時計回りで、どちらもビューアから離れる方向を指します。
				//頂点法線が割り当てられている、それらは一般的に指す必要があります
				//サーフェス法線の方向。それ以外の場合は予測できない結果
				//発生する可能性があります。
				//
				//面にテクスチャマップが割り当てられており、テクスチャ頂点がない場合
				// fステートメントで割り当てられている場合、テクスチャマップは無視されます
				//要素がレンダリングされます。
				for (u_int i = 0; i < 3; i++)
				{
					Vertex vertex;
					u_int v, vt, vn;

					fin >> v;
					vertex.position = positions[v - 1];
					if (L'/' == fin.peek())
					{
						fin.ignore();
						if (L'/' != fin.peek())
						{
							fin >> vt;
							// UNIT.13
							vertex.texcoord = texcoords[vt - 1];
						}
						if (L'/' == fin.peek())
						{
							fin.ignore();
							fin >> vn;
							vertex.normal = normals[vn - 1];
						}
					}
					vertices.push_back(vertex);
					indices.push_back(current_index++);
				}
				fin.ignore(1024, '\n');
			}

			else if (0 == wcscmp(command, L"mtllib"))
			{
				// mtllib filename1 filename2。 。 。
				//マテリアル定義のマテリアルライブラリファイルを指定します
				// usemtlステートメントで設定します。複数のファイル名を指定できます
				// mtllibを使用します。複数のファイル名が指定されている場合、最初のファイル
				//リストは最初に材料定義を検索し、2番目に検索されます
				//ファイルが次に検索され、以下同様です。
				wchar_t mtllib[256];
				fin >> mtllib;
				mtl_filenames.push_back(mtllib);
			}

			else if (0 == wcscmp(command, L"usemtl"))
			{
				wchar_t usemtl[MAX_PATH] = { 0 };
				fin >> usemtl;

				Subset current_subset = {};
				current_subset.usemtl = usemtl;

				current_subset.index_start = static_cast<u_int>(indices.size());
				subsets.push_back(current_subset);
			}
			else
			{
				fin.ignore(1024, '\n');
			}
		}
		fin.close();
	}

	std::vector<Subset>::reverse_iterator iterator = subsets.rbegin();
	iterator->index_count = static_cast<u_int>(indices.size() - iterator->index_start);
	for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); iterator++)
	{
		iterator->index_count = (iterator - 1)->index_start - iterator->index_start;
	}

	CreateBuffers(device, vertices.data(), /*ToDo*/vertices.size(), indices.data(), indices.size());

	//std::wstring texture_filename;
	// parse MTL file
	{
		wchar_t mtl_filename[256];
		/*
		e.g.
		obj_filename <= L"data/bison.obj"
		resource_filename <= L"/user/textures/bison.png"
		combined_resource_path => L"/data/bison.png"
		*/
		CombineResourcePath(mtl_filename, objFilename, mtl_filenames[0].c_str());

		std::wifstream fin(mtl_filename);
		_ASSERT_EXPR(fin, L"'MTL file not fuund.");

		wchar_t command[256] = { 0 };
		while (fin)
		{
			fin >> command;
			if (0 == wcscmp(command, L"#"))
			{
				// Comment
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"map_Kd"))
			{
				// map_Kd-オプションの引数ファイル名
				//
				//カラーテクスチャファイルまたはカラープロシージャルテクスチャファイルが
				//マテリアルの拡散反射率にリンクされます。レンダリング中、
				// map_Kd値はKd値で乗算されます。
				//
				// "filename"はカラーテクスチャファイル（.mpc）の名前、色
				//手続き型テクスチャファイル（.cxc）、または画像ファイル。
				fin.ignore();
				wchar_t map_Kd[256];
				fin >> map_Kd;
				CombineResourcePath(map_Kd, objFilename, map_Kd);
				//texture_filename = map_Kd;
				materials.rbegin()->map_Kd = map_Kd;
				fin.ignore(1024, L'\n');
			}

			else if (0 == wcscmp(command, L"newmtl"))
			{
				//以下の構文は、マテリアル名ステートメントを記述しています。
				//
				// newmtl名
				//
				//マテリアルの説明の開始を指定し、名前を
				// material.An.mtlファイルには、先頭に1つのnewmtlステートメントが必要です
				//各マテリアルの説明。
				// "name"はマテリアルの名前です。名前は任意の長さですが
				//ブランクを含めることはできません。アンダースコアはマテリアル名で使用できます。マテリアルマテリアル;
				fin.ignore();
				wchar_t newmtl[256];
				Material material;
				fin >> newmtl;
				material.newmtl = newmtl;
				materials.push_back(material);
			}

			else if (0 == wcscmp(command, L"Ka"))
			{
				// Ka r g b
				//
				// Kaステートメントは、RGB値を使用して周囲の反射率を指定します。
				// "r g b"は、赤、緑、青のコンポーネントの値です
				// color。gおよびb引数はオプションです。rのみが指定されている場合、
				//次に、gとbはrと等しいと想定されます。rg bの値は
				//通常は0.0から1.0の範囲です。この範囲外の値は増加します
				//または、それに応じて反射率を下げます。
				float r, g, b;
				fin >> r >> g >> b;
				materials.rbegin()->Ka = DirectX::XMFLOAT3(r, g, b);
				fin.ignore(1024, L'\n');
			}

			else if (0 == wcscmp(command, L"Kd"))
			{
				// Kd r g b
				//
				// Kdステートメントは、RGB値を使用して拡散反射率を指定します。
				// "r g b"は、赤、緑、青のコンポーネントの値です
				// atmosphere。gおよびb引数はオプションです。rのみが
				//指定すると、gおよびbはrと等しいと見なされます。rg b値
				//通常は0.0から1.0の範囲です。この範囲外の値
				//それに応じて反射率を増減します。
				float r, g, b;
				fin >> r >> g >> b;
				materials.rbegin()->Kd = DirectX::XMFLOAT3(r, g, b);
				fin.ignore(1024, L'\n');
			}

			else if (0 == wcscmp(command, L"Ks"))
			{
				// Ks r g b
				//
				// Ksステートメントは、RGB値を使用して鏡面反射を指定します。
				// "r g b"は、赤、緑、青のコンポーネントの値です
				// atmosphere。gおよびb引数はオプションです。rのみが
				//指定すると、gおよびbはrと等しいと見なされます。rg b値
				//通常は0.0から1.0の範囲です。この範囲外の値
				//それに応じて反射率を増減します。
				float r, g, b;
				fin >> r >> g >> b;
				materials.rbegin()->Ks = DirectX::XMFLOAT3(r, g, b);
				fin.ignore(1024, L'\n');
			}

			else if (0 == wcscmp(command, L"illum"))
			{
				// illum illum_＃
				//
				// "illum"ステートメントは、で使用する照明モデルを指定します
				// material.Illuminationモデルは、
				//さまざまなマテリアルのライティングとシェーディングエフェクト。
				//
				// "illum_＃"には0〜10の数値を指定できます。照明モデルは
				//以下にまとめます;
				// 0カラーオン、アンビエントオフ
				// 1色がオンで、周囲光がオン
				// 2ハイライト
				// 3反射をオン、光線追跡をオン
				// 4透明度：Glassオン
				//反射：光線追跡
				// 5反射：フレネルをオン、レイトレースをオン
				// 6透明度：屈折
				//反射：フレネルオフ、レイトレースオン
				// 7透明度：屈折
				//反射：フレネルをオンにしてレイトレースをオンに
				// 8反射がオンで光線追跡がオフ
				// 9透明度：Glass on
				//反射：光線追跡オフ
				// 10影を不可視にキャストします
				u_int illum;
				fin >> illum;
				materials.rbegin()->illum = illum;
				fin.ignore(1024, L'\n');
			}
			else
			{
				// Unimplemented or unrecognized command
				fin.ignore(1024, L'\n');
			}
		}
	}

	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	CreateVertexShaderFromCso(device, "Shaders/static_mesh_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	CreatePixelShaderFromCso(device, "Shaders/static_mesh_ps.cso", pixel_shader.GetAddressOf());

	// create rasterizer state : solid mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
	// create rasterizer state : wireframe mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
	// create depth stencil state
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		depth_stencil_desc.StencilEnable = FALSE;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
	// create constant buffer
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(ConstantBuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	D3D11_TEXTURE2D_DESC texture2d_desc;

	//hr = loat_texture_from_file(device, texture_filename.c_str(), &shader_resource_view, &texture2d_desc);
	for (auto& material : materials)
	{
		LoadTextureFromFile(device, material.map_Kd.c_str(), &material.shader_resource_view, &texture2d_desc);
	}

	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//ToDo SetBorderColor
	sampler_desc.BorderColor[0] = 0.f;
	sampler_desc.BorderColor[1] = 0.f;
	sampler_desc.BorderColor[2] = 0.f;
	sampler_desc.BorderColor[3] = 0.f;
	//memcpy(sampler_desc.BorderColor, &DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(DirectX::XMFLOAT4));
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampler_desc, sampler_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices)
{
	HRESULT hr;
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(Vertex) * numVertices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = vertices;
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.mm
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.

		hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(u_int) * numIndices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = indices;
		subresource_data.SysMemPitch = 0; //Not use for index buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
}

void StaticMesh::Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& worldViewProjection, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor, bool wireframe)
{
	u_int stride = sizeof(Vertex);
	u_int offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	immediateContext->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(input_layout.Get());

	immediateContext->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediateContext->PSSetShader(pixel_shader.Get(), nullptr, 0);

	immediateContext->OMSetDepthStencilState(depth_stencil_state.Get(), 1);
	if (wireframe)
	{
		immediateContext->RSSetState(rasterizer_states[1].Get());
	}
	else
	{
		immediateContext->RSSetState(rasterizer_states[0].Get());
	}
	for (Material& material : materials)
	{
		ConstantBuffer data;
		data.world_view_projection = worldViewProjection;
		data.world = world;
		data.light_direction = lightDirection;
		data.material_color.x = materialColor.x * material.Kd.x;
		data.material_color.y = materialColor.y * material.Kd.y;
		data.material_color.z = materialColor.z * material.Kd.z;
		data.material_color.w = materialColor.w;
		immediateContext->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
		immediateContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

		immediateContext->PSSetShaderResources(0, 1, material.shader_resource_view.GetAddressOf());
		immediateContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
		for (Subset& subset : subsets)
		{
			if (material.newmtl == subset.usemtl)
			{
				immediateContext->DrawIndexed(subset.index_count, subset.index_start, 0);
			}
		}
	}
}
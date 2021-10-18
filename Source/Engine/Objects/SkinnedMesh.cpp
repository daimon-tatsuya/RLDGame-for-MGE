#include <vector>
#include <functional>
#include <fbxsdk.h>

#include "Engine/Systems/misc.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Texture.h"
#include "Engine/Objects/SkinnedMesh.h"

using namespace fbxsdk;
//Todo 警告の量が多いので(直すのが面倒だから)プロジェクトから除外する

struct BoneInfluence
{
	int index;    //indexのbone
	float weight; //weightのbone
};

typedef std::vector<BoneInfluence> bone_influences_per_control_point;

void FbxaMatrixToXmfloat4x4(const FbxAMatrix& fbxamatrix, DirectX::XMFLOAT4X4& xmfloat4x4)
{
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
		}
	}
}

//ボーン影響度をFBXデータから取得する関数
void FetchBoneInfluence(const FbxMesh* fbxMesh, std::vector<bone_influences_per_control_point>& influences)
{
	const int number_of_control_points = fbxMesh->GetControlPointsCount();
	influences.resize(number_of_control_points);

	const int number_of_deformers = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int index_of_defomer = 0; index_of_defomer < number_of_deformers; index_of_defomer++)
	{
		FbxSkin* skin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(index_of_defomer, FbxDeformer::eSkin));
		const int number_of_clusters = skin->GetClusterCount();
		for (int index_of_cluster = 0; index_of_cluster < number_of_clusters; index_of_cluster++)
		{
			FbxCluster* cluster = skin->GetCluster(index_of_cluster);
			const int number_of_control_point_indices = cluster->GetControlPointIndicesCount();
			const int* array_of_control_point_indices = cluster->GetControlPointIndices();
			const double* array_of_control_point_weights = cluster->GetControlPointWeights();

			for (int i = 0; i < number_of_control_point_indices; i++)
			{
				bone_influences_per_control_point& influences_per_control_point
					= influences.at(array_of_control_point_indices[i]);
				BoneInfluence influence;
				influence.index = index_of_cluster;
				influence.weight = static_cast<float>(array_of_control_point_weights[i]);
				influences_per_control_point.push_back(influence);
			}
		}
	}
}

//ボーン行列をFBX データから取得する関数
void FetchBoneMatrices(FbxMesh* fbxMesh, std::vector<SkinnedMesh::Bone>& skeletal, FbxTime time)
{
	const int number_of_deformers = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int index_of_deformer = 0; index_of_deformer < number_of_deformers; index_of_deformer++)
	{
		FbxSkin* skin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(index_of_deformer, FbxDeformer::eSkin));

		//FbxSkin::EType skinning_type = skin->GetSkinningType();
		//_ASSERT_EXPR(skinning_type == FbxSkin::eLinear || skinning_type == FbxSkin::eRigid, L"only support FbxSkin::eLinear or FbxSkin::eRigid");

		//FbxCluster::ELinkMode link_mode = skin->GetCluster(0)->GetLinkMode();
		//_ASSERT_EXPR(link_mode != FbxCluster::eAdditive, L"only support FbxCluster::eNormalize or FbxCluster::eTotalOne");

		const int number_of_clusters = skin->GetClusterCount();
		skeletal.resize(number_of_clusters);
		for (int index_of_cluster = 0; index_of_cluster < number_of_clusters; index_of_cluster++)
		{
			SkinnedMesh::Bone& bone = skeletal.at(index_of_cluster);

			FbxCluster* cluster = skin->GetCluster(index_of_cluster);

			// this matrix trnasforms coordinates of the initial pose from mesh space to global space
			FbxAMatrix reference_global_init_position;
			cluster->GetTransformMatrix(reference_global_init_position);

			//const FbxVector4 T = fbx_mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
			//const FbxVector4 R = fbx_mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
			//const FbxVector4 S = fbx_mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
			//FbxAMatrix reference_geometry(T, R, S);
			//reference_global_init_position *= reference_geometry;

			// this matrix trnasforms coordinates of the initial pose from bone space to global space
			//初期ポーズの座標をボーン空間からグローバル空間に変換
			FbxAMatrix cluster_global_init_position;
			cluster->GetTransformLinkMatrix(cluster_global_init_position);

			// this matrix trnasforms coordinates of the current pose from bone space to global space
			//現在のポーズの座標をボーン空間からグローバル空間に変換
			FbxAMatrix cluster_global_current_position;
			cluster_global_current_position = cluster->GetLink()->EvaluateGlobalTransform(time);

			// this matrix trnasforms coordinates of the current pose from mesh space to global space
			//現在のポーズの座標をメッシュ空間からグローバル空間に変換
			FbxAMatrix reference_global_current_position;
			reference_global_current_position = fbxMesh->GetNode()->EvaluateGlobalTransform(time);

			// Matrices are defined using the Column Major scheme. When a FbxAMatrix represents a transformation (translation, rotation and scale), the last row of the matrix represents the translation part of the transformation.
			//行列は、列メジャースキームを使用して定義。 FbxAMatrixが変換（変換、回転、スケール）を表す場合、行列の最後の行は変換の変換部分を表す。
			FbxAMatrix transform = reference_global_current_position.Inverse() * cluster_global_current_position * cluster_global_init_position.Inverse() * reference_global_init_position;

			FbxaMatrixToXmfloat4x4(transform, bone.transform);
		}
	}
}

//アニメーションを取得する関数
void FetchAnimations(FbxMesh* fbxMesh, SkinnedMesh::SkeletalAnimation& skeletalAnimation,
	u_int sampling_rate = 0)
{
	// Get the list of all the animation stack.
	//すべてのアニメーションスタックのリストを取得。
	FbxArray<FbxString*> array_of_animation_stack_names;
	fbxMesh->GetScene()->FillAnimStackNameArray(array_of_animation_stack_names);
	// Get the number of animations.
	//アニメーションの数を取得。
	int number_of_animations = array_of_animation_stack_names.Size();
	if (number_of_animations > 0)
	{
		// Get the FbxTime per animation's frame.
		//アニメーションのフレームごとにFbxTimeを取得。
		FbxTime::EMode time_mode = fbxMesh->GetScene()->GetGlobalSettings().GetTimeMode();
		FbxTime frame_time;
		frame_time.SetTime(0, 0, 0, 1, 0, time_mode);
		sampling_rate = sampling_rate > 0 ? sampling_rate : static_cast<u_int>(frame_time.GetFrameRate(time_mode));
		float sampling_time = 1.0f / sampling_rate;
		skeletalAnimation.sampling_time = sampling_time;
		skeletalAnimation.animation_tick = 0.0f;
		FbxString* animation_stack_name = array_of_animation_stack_names.GetAt(0);
		FbxAnimStack* current_animation_stack
			= fbxMesh->GetScene()->FindMember<FbxAnimStack>(animation_stack_name->Buffer());
		fbxMesh->GetScene()->SetCurrentAnimationStack(current_animation_stack);
		FbxTakeInfo* take_info = fbxMesh->GetScene()->GetTakeInfo(animation_stack_name->Buffer());
		FbxTime start_time = take_info->mLocalTimeSpan.GetStart();
		FbxTime end_time = take_info->mLocalTimeSpan.GetStop();
		FbxTime sampling_step;
		sampling_step.SetTime(0, 0, 1, 0, 0, time_mode);
		sampling_step = static_cast<FbxLongLong>(sampling_step.Get() * sampling_time);
		for (FbxTime current_time = start_time; current_time < end_time; current_time += sampling_step)
		{
			SkinnedMesh::skeletal skeletal;
			FetchBoneMatrices(fbxMesh, skeletal, current_time);
			skeletalAnimation.push_back(skeletal);
		}
	}
	for (int i = 0; i < number_of_animations; i++)
	{
		delete array_of_animation_stack_names[i];
	}
}
SkinnedMesh::SkinnedMesh(ID3D11Device* device, const char* fbxFilename)
{
	// FBX SDKマネージャーを作成
	FbxManager* manager = FbxManager::Create();

	// OSettingsオブジェクトを作成
	// IOSROOTはFbxiosettingspath.hで定義
	manager->SetIOSettings(FbxIOSettings::Create(manager, IOSROOT));

	// インポーターを作成
	FbxImporter* importer = FbxImporter::Create(manager, "");

	// インポーターの初期化
	bool import_status = false;
	import_status = importer->Initialize(fbxFilename, -1, manager->GetIOSettings());
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());

	// 新しいシーンを作成
	// インポートされたファイルを入力できるようにする
	FbxScene* scene = FbxScene::Create(manager, "");

	//ファイルのコンテンツをシーンにインポート
	import_status = importer->Import(scene);
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());

	//メッシュ、NURBS、パッチを三角形メッシュに変換
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene, /*replace*/true);

	// このノードの下のノード属性とマテリアルを再帰的に取得
	// 現在はメッシュのみ
	std::vector<FbxNode*> fetched_meshes;
	std::function<void(FbxNode*)> traverse = [&](FbxNode* node)
	{
		if (node)
		{
			FbxNodeAttribute* fbx_node_attribute = node->GetNodeAttribute();
			if (fbx_node_attribute)
			{
				switch (fbx_node_attribute->GetAttributeType())
				{
				case FbxNodeAttribute::eMesh:
					fetched_meshes.push_back(node);
					break;
				}
			}
			for (int i = 0; i < node->GetChildCount(); i++)
			{
				traverse(node->GetChild(i));
			}
		}
	};
	traverse(scene->GetRootNode());

	meshes.resize(fetched_meshes.size());

	for (size_t i = 0; i < fetched_meshes.size(); i++)
	{
		FbxNode* fbx_Node = fetched_meshes.at(i);
		FbxMesh* fbx_mesh = fbx_Node->GetMesh();
		Mesh& mesh = meshes.at(i);

		FbxAMatrix global_transform = fbx_mesh->GetNode()->EvaluateGlobalTransform(0);

		FbxaMatrixToXmfloat4x4(global_transform, mesh.global_transform);

		// 材料特性を取得
		const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();

		mesh.subsets.resize(number_of_materials > 0 ? number_of_materials : 1);

		for (int index_of_material = 0; index_of_material < number_of_materials; index_of_material++)
		{
			Subset& subset = mesh.subsets.at(index_of_material);

			const FbxSurfaceMaterial* surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);

			const FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			const FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (property.IsValid() && factor.IsValid())
			{
				FbxDouble3 color = property.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				subset.diffuse.color.x = static_cast<float>(color[0] * f);
				subset.diffuse.color.y = static_cast<float>(color[1] * f);
				subset.diffuse.color.z = static_cast<float>(color[2] * f);
				subset.diffuse.color.w = 1.0f;
			}
			if (property.IsValid())
			{
				const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
				if (number_of_textures)
				{
					const FbxFileTexture* file_texture = property.GetSrcObject<FbxFileTexture>();
					if (file_texture)
					{
						const char* filename = file_texture->GetRelativeFileName();
						// 「ファイル名」から「diffuse.shader_resource_view」を作成

						wchar_t fbx_unicode[256];
						MultiByteToWideChar(CP_ACP, 0, fbxFilename, /*Todo*/strlen(fbxFilename) + 1, fbx_unicode, 1024);
						wchar_t texture_unicode[256];
						MultiByteToWideChar(CP_ACP, 0, file_texture->GetFileName(), /*Todo*/strlen(file_texture->GetFileName()) + 1, texture_unicode, 1024);
						CombineResourcePath(texture_unicode, fbx_unicode, texture_unicode);

						//material.texture_filename = texture_unicode;
						D3D11_TEXTURE2D_DESC texture2d_desc;
						LoadTextureFromFile(device, texture_unicode, subset.diffuse.shader_resource_view.GetAddressOf(), &texture2d_desc);
					}
				}
			}
		}

		for (Subset& subset : mesh.subsets)
		{
			if (!subset.diffuse.shader_resource_view)
			{
				MakeDummyTexture(device, subset.diffuse.shader_resource_view.GetAddressOf());
			}
		}

		//各マテリアルのポリゴン数を数える
		if (number_of_materials > 0)
		{
			//各マテリアルの面を数える
			const int number_of_polygons = fbx_mesh->GetPolygonCount();
			for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
			{
				const u_int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);

				mesh.subsets.at(material_index).index_count += 3;
			}

			//オフセットを記録（頂点の数）
			int offset = 0;

			for (Subset& subset : mesh.subsets)
			{
				subset.index_start = offset;
				offset += subset.index_count;
				// これは、次の手順でカウンターとして使用され、ゼロにリセットされる
				subset.index_count = 0;
			}
		}

		//メッシュデータを取得
		std::vector<Vertex> vertices;
		std::vector<u_int> indices;
		u_int vertex_count = 0;

		//
		FbxStringList uv_names;
		fbx_mesh->GetUVSetNames(uv_names);

		const FbxVector4* array_of_control_points = fbx_mesh->GetControlPoints();
		const int number_of_polygons = fbx_mesh->GetPolygonCount();

		indices.resize(number_of_polygons * 3);

		std::vector<bone_influences_per_control_point> bone_influences;
		FetchBoneInfluence(fbx_mesh, bone_influences);

		for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
		{
			// 現在の面のマテリアル
			int index_of_material = 0;
			if (number_of_materials > 0)
			{
				index_of_material = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
			}

			Subset& subset = mesh.subsets.at(index_of_material);
			const int index_offset = subset.index_start + subset.index_count;

			for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++)
			{
				Vertex vertex;
				const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
				vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
				vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
				vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);

				bone_influences_per_control_point influences_per_control_point = bone_influences.at(index_of_control_point);
				for (size_t index_of_influence = 0; index_of_influence < influences_per_control_point.size(); index_of_influence++)
				{
					if (index_of_influence < MAX_BONE_INFLUENCES)
					{
						vertex.bone_weights[index_of_influence] = influences_per_control_point.at(index_of_influence).weight;
						vertex.bone_indices[index_of_influence] = influences_per_control_point.at(index_of_influence).index;
					}
				}

				if (fbx_mesh->GetElementNormalCount() > 0)
				{
					FbxVector4 normal;
					fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				if (fbx_mesh->GetElementUVCount() > 0)
				{
					FbxVector2 uv;
					bool unmapped_uv;
					fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}

				vertices.push_back(vertex);

				indices.at(index_offset + index_of_vertex) = static_cast<u_int>(vertex_count);
				vertex_count += 1;
			}
			subset.index_count += 3;
		}

		FetchAnimations(fbx_mesh, mesh.skeletal_animation);
		CreateBuffers(device, vertices.data(), /*Todo*/vertices.size(), indices.data(), indices.size());
	}

	manager->Destroy();

	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "WEIGHTS",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",     0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	CreateVertexShaderFromCso(device, "Shaders/skinned_mesh_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	CreatePixelShaderFromCso(device, "Shaders/skinned_mesh_ps.cso", pixel_shader.GetAddressOf());

	// create rasterizer state : solid mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK
		rasterizer_desc.FrontCounterClockwise = TRUE;
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
		rasterizer_desc.FrontCounterClockwise = TRUE;
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

	// create sampler state
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//Todo SetBorderColor
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
void SkinnedMesh::CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices)
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

void SkinnedMesh::Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& worldViewProjection, const DirectX::XMFLOAT4X4& worldInverseTranspose, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor, bool wireframe, float elapsed_time/*UNIT.23*/)
{
	for (Mesh& mesh : meshes)
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

		ConstantBuffer data;

		DirectX::XMStoreFloat4x4(&data.world_view_projection,
			DirectX::XMLoadFloat4x4(&mesh.global_transform) *
			DirectX::XMLoadFloat4x4(&coordinate_conversion) *
			DirectX::XMLoadFloat4x4(&worldViewProjection));

		DirectX::XMStoreFloat4x4(&data.world_inverse_transpose,
			DirectX::XMLoadFloat4x4(&mesh.global_transform) *
			DirectX::XMLoadFloat4x4(&coordinate_conversion) *
			DirectX::XMLoadFloat4x4(&worldInverseTranspose));

		data.light_direction = lightDirection;

		if (mesh.skeletal_animation.size() > 0)
		{
			size_t frame = static_cast<size_t>(mesh.skeletal_animation.animation_tick / mesh.skeletal_animation.sampling_time);
			if (frame > mesh.skeletal_animation.size() - 1)
			{
				frame = 0;
				mesh.skeletal_animation.animation_tick = 0;
			}
			std::vector<Bone>& skeletal = mesh.skeletal_animation.at(frame);
			size_t number_of_bones = skeletal.size();
			_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");
			for (size_t i = 0; i < number_of_bones; i++)
			{
				DirectX::XMStoreFloat4x4(&data.bone_transforms[i], DirectX::XMLoadFloat4x4(&skeletal.at(i).transform));
			}
			mesh.skeletal_animation.animation_tick += elapsed_time;
		}

		for (Subset& subset : mesh.subsets)
		{
			data.material_color.x = subset.diffuse.color.x * materialColor.x;
			data.material_color.y = subset.diffuse.color.y * materialColor.y;
			data.material_color.z = subset.diffuse.color.z * materialColor.z;
			data.material_color.w = materialColor.w;
			immediateContext->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
			immediateContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

			immediateContext->PSSetShaderResources(0, 1, subset.diffuse.shader_resource_view.GetAddressOf());
			immediateContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
			immediateContext->DrawIndexed(subset.index_count, subset.index_start, 0);
		}
	}
}
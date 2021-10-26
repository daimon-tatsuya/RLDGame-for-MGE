#pragma once

#include <string>
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <fbxsdk.h>

class ModelResource
{
public:
	ModelResource(ID3D11Device* device, const char* filename, const char* ignoreRootMotionNodeName = nullptr);
	~ModelResource() {}

	// Node = Boneと考えていい
	struct Node
	{
		std::string						name;
		int									parent_index;
		DirectX::XMFLOAT3		scale;			// ローカルの変換行列?
		DirectX::XMFLOAT4		rotate;
		DirectX::XMFLOAT3		translate;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Material
	{
		DirectX::XMFLOAT4													color = { 0.8f, 0.8f, 0.8f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view = nullptr;	// シリアライズの対象外
		std::string																	texture_filename = "";		// モデルを読み込んだ時にテクスチャを作成する

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	// マテリアルの設定?
	struct Subset
	{
		unsigned int		start_index = 0;
		unsigned int		index_count = 0;
		unsigned int		material_index = 0;
		Material*			material = nullptr;	// モデルを読み込んだ時にマテリアルを関連ずける

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	// それぞれの頂点
	struct Vertex
	{
		DirectX::XMFLOAT3		position{};	// ローカル座標
		DirectX::XMFLOAT3		normal{};		// ローカル座標
		DirectX::XMFLOAT2		texcoord{};		// UV coordinates
		DirectX::XMFLOAT4		bone_weight = { 1.0f,0.0f,0.0f,0.0f };
		DirectX::XMUINT4			bone_index = { 0,0,0,0 };

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer;	// シリアライズの対象外
		Microsoft::WRL::ComPtr<ID3D11Buffer>	index_buffer;	// シリアライズの対象外

		std::vector<Vertex>								vertices;
		std::vector<UINT>								indices;
		std::vector<Subset>								subsets;
		std::string											name;
		int														node_index;
		std::vector<int>									node_indices;
		std::vector<DirectX::XMFLOAT4X4>		inverse_transforms;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct NodeKeyData
	{
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Keyframe
	{
		float									seconds;
		std::vector<NodeKeyData>	node_keys;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};
	struct Animation
	{
		float								seconds_length;
		std::vector<Keyframe>		keyframes;
		//	std::string					name;
		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	void AddAnimation(const char* fbxFilename);

	const std::vector<Mesh>& GetMeshes() const { return meshes; }
	const std::vector<Node>& GetNodes() const { return nodes; }
	const std::vector<Animation>& GetAnimations() const { return animations; }

private:
	// モデルを構築
	void BuildModel(ID3D11Device* dev1vbnice, const char* fbxFilename);

	//シリアライズされたモデル構築
	void BuildSerializedModel(ID3D11Device* device, const char* dirname);

	// ノードデータを構築
	void BuildNodes(FbxNode* fbxNode, int parentNodeIndex);
	void BuildNode(FbxNode* fbxNode, int parentNodeIndex);

	// メッシュデータを構築
	void BuildMeshes(ID3D11Device* device, FbxNode* fbxNode);
	void BuildMesh(ID3D11Device* device, FbxNode* fbxNode, FbxMesh* fbxMesh);

	// マテリアルデータを構築
	void BuildMaterials(ID3D11Device* device, const char* dirname, FbxScene* fbxScene);
	void BuildMaterial(ID3D11Device* device, const char* dirname, FbxSurfaceMaterial* fbxSurfaceMaterial);

	// アニメーションデータを構築
	void BuildAnimations(FbxScene* fbxScene);

	// インデックスの検索
	int FindNodeIndex(const char* name);
	int FindMaterialIndex(FbxScene* fbxScene, const FbxSurfaceMaterial* fbxSurfaceMaterial);

	// シリアライズ
	void Serialize(const char* filename);

	// デシリアライズ
	void Deserialize(const char* filename);

	void Load(ID3D11Device* device, const char* filename);

	// フルパス名取得
	std::string GetFullPathNodeName(Node& node);
private:
	std::vector<Node>			nodes;
	std::vector<Material>		materials;
	std::vector<Mesh>			meshes;
	std::vector<Animation>	animations;
	int									root_motion_node_index = -1;
	const char*						ignore_root_motion_node_name;
};

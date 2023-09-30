// モデル
#pragma once
#include "main.h"
#include "Shader.h"
#include <vector>
#include "DXBuffer.h"
#include <memory>
#include <map>
#include <string>
#include "ConstantBuffer.h"
#include "Calc.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define SHADER_BONE_NUM (64)			// シェーダーに渡す最大骨数
#define MAX_INSTANCING (100)			// インスタンシング描画最大数

class Animation
{
public:
	struct VectorKey
	{
		XMFLOAT3 Value;
		float Time;
	};

	struct QuaternionKey
	{
		Quaternion Value;
		float Time;
	};

	struct AnimeInfo
	{
		std::string NodeName;			// アニメーションするノードの名前
		int nNumPositionKeys;			// 位置アニメーションのキーの数
		VectorKey* PositionKeys;		// 位置キーのデータ
		int nNumRotateKeys;				// 回転アニメーションのキーの数
		QuaternionKey* RotateKeys;		// 回転キーのデータ
		int nNumScaleKeys;				// 拡縮キーの数
		VectorKey* ScaleKeys;			// 拡縮キーデータ
	};

	struct BoneMatrix
	{
		XMFLOAT3 transrate;
		Quaternion rotation;
		XMFLOAT3 scaling;
		BoneMatrix()
		{
			transrate = { 0.0f,0.0f,0.0f };
			rotation = { 0.0f,0.0f,0.0f,1.0f };
			scaling = { 1.0f,1.0f,1.0f };
		}
	};
public:
	Animation(const aiAnimation*);
	~Animation();

	void AnimeEvalution(float & time);
	float CalcPlayFrame(float& time);
	void CalcAnime(float & frame);

	void GetLocalMatrix(XMMATRIX* ,std::string);
	void GetLocalPosition(XMFLOAT3*, std::string);
	void GetLocalScaling(XMFLOAT3*, std::string);
	void GetLocalRotation(Quaternion*, std::string);

	inline float& GetDuration();
	inline float& GetTick();
private:
	void LoadAnimeData(const aiAnimation*);

private:
	std::map<std::string, XMMATRIX> NodeMatrix;			// ローカルの姿勢をノード名から算出する
	std::map<std::string, BoneMatrix> BoneNodeMatrix;		// ローカルの姿勢(アニメーションブレンド用に分割したもの)

	std::vector<XMFLOAT4X4> m_Transforms;
	std::vector<BoneMatrix> m_Brends;
	float mLastTime;										// 最期のアニメ更新時間

	float m_Tick;
	float m_Duration;

	int m_NumChannels;
	AnimeInfo* m_Channels;									// アニメーション情報リスト
};

class Model
{
public:
	struct Vertex {
		XMFLOAT3 pos;				// 頂点座標
		XMFLOAT3 normal;			// 法線
		XMFLOAT2 uv;				// テクスチャ座標
		// ボーンの結びつき、影響
		float weight[4];			// ボーンによる影響
		unsigned int index[4];		// どの骨に結び付くか
	};

	struct Material {
		XMFLOAT4 diffuse;						// 拡散光
		XMFLOAT4 ambient;						// 環境光
		XMFLOAT4 specular;						// 鏡面反射
		ID3D11ShaderResourceView* pTexture;		// テクスチャ
	};

	struct Mesh {
		Vertex* Vertices;
		int NumVertex;
		unsigned int* Indices;
		unsigned int materialID;
		int NumIndex;
		DXBuffer* pMeshBuffer;
		bool hasBone;
		std::string name;
	};
	struct NodeInfo {
		NodeInfo* parent;
		std::string name;
		std::vector<NodeInfo*> Children;
		XMMATRIX LocalTransformation;
		XMMATRIX Transformation;
	};

	struct BoneInfo {
		XMMATRIX offsetMatrix;					// オフセット行列(逆バインドポーズをとるための行列)
		XMMATRIX InverseMatrix;					// オフセット行列を打ち消す行列
		XMMATRIX FinalTransformation;			// 最終的に計算された骨の行列
		int index;								// 骨番号
		BoneInfo* parent;						// 親の骨
		std::vector<BoneInfo*> Children;		// 子の骨
	};

public:
	Model();
	~Model();
	bool virtual Load(const char* filename, float scale = 1,bool flip = true);
	void AddAnime(const char* filename,bool flip = true);


	void Draw();
	void Draw(XMFLOAT4X4 mtx);
	void DrawInstancing(XMFLOAT4X4* mtx);
	void PlayAnime(float& time, int* animeindex);
	void PlayBlendAnime(float& time, int* animeindex, const int& blendindex = -1,const float& rate = 0.0f);
	float GetAnimDuration(int animeindex);
	float GetAnimTick(int animeindex);
	const XMMATRIX& GetBoneMatrix(std::string bonename);

private:			// ===公開しない関数

	void LoadMesh(const aiScene* pScene, float scale = 1.0f);
	void LoadMaterial(const aiScene* pScene, const char* filename);

	void CreateBoneNode(const aiNode* node, std::string name);
	NodeInfo* CreateNode(const aiNode* node);


	void UpdateNodeTransform(NodeInfo* pNode,XMMATRIX* nodematrix);
	void CalcGlobalTransform();			// ルートノードから辿り、全ての骨の最終的な変換行列を求める

	void UpdateTransforms(NodeInfo* pNode,int& animeidx);							// アニメーション更新
	void UpdateTransforms(NodeInfo* pNode, int& animeidx, int& blendidx ,const float& rate);			// 遷移アニメーション更新
protected:
	
private:
	static ID3D11SamplerState* m_sampler;				// テクスチャサンプラ
	static int m_ModelNum;

	std::vector<BoneInfo*> m_BoneList;					// 骨リスト
	std::map<std::string, BoneInfo*> m_BoneNode;		// 骨のノードマップ(骨のオフセット行列、骨のインデックスを保持する)
	std::map<std::string, NodeInfo*> m_NodeMap;			// Assimpで読み込んだノード情報(モデルのメッシュや骨の階層を保持する)
	std::map<std::string, Animation*> m_AnimeMap;		// モデルから読み込んだアニメーションマップ(ファイル名)

	Mesh* m_pMeshes;									// メッシュ
	Material* m_pMaterials;								// マテリアル
	int m_MeshNum;										// メッシュの数
	int m_MaterialNum;									// マテリアルの数
	int m_BoneNum;										// 骨の数
	ConstantBuffer* m_BoneBuffer;						// 定数バッファ(頂点シェーダ―に渡す骨の行列など)
	ConstantBuffer* m_InstancingBuffer;					// 定数バッファ(インスタンシング描画用)
	ConstantBuffer* m_WVP;
	NodeInfo* m_Root;			// ノードを親からたどる必要があるため先頭ノードを持っておく

	std::vector<Animation*> m_pAnimList;

	XMFLOAT4X4 m_GlobalInverseTransform;
	int m_nCurrentAnime;
	int m_nBlendAnime;
};

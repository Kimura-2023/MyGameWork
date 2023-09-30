// ===モデルクラス実装===
// Assimpで読み込んだモデルを描画するクラス。
// Author: kimura

// ===インクルード===
#include "Model.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <DirectXTex.h>
#include "Texture.h"
#include "ShaderList.h"
#include "CameraManager.h"
#include "TextureManager.h"

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc141-mtd")
#pragma comment(lib, "zlibstaticd")
#else
#pragma comment(lib, "assimp-vc141-mt")
#pragma comment(lib, "zlibstatic")
#endif

// ===関数宣言===
XMFLOAT4X4 ConvertaiMatrix(const aiMatrix4x4& m);

// ===グローバル宣言===
ID3D11SamplerState* Model::m_sampler = nullptr;
int Model::m_ModelNum = 0;

// ===アニメクラスコンストラクタ===
Animation::Animation(const aiAnimation* panim) : mLastTime(0)
{
	LoadAnimeData(panim);
}

// ===アニメクラスデストラクタ===
Animation::~Animation()
{
	for (int i = 0; i < m_NumChannels; ++i)
	{
		delete[] m_Channels[i].PositionKeys;
		delete[] m_Channels[i].RotateKeys;
		delete[] m_Channels[i].ScaleKeys;
	}
	if(m_Channels)
		delete[] m_Channels;
}

// ===Assimpで読み込んだデータからアニメーションの情報を読み込む===
void Animation::LoadAnimeData(const aiAnimation* panim)
{
	m_Tick = panim->mTicksPerSecond;
	m_Duration = panim->mDuration;
	m_NumChannels = panim->mNumChannels;

	m_Channels = new AnimeInfo[m_NumChannels];			// チャンネル(アニメーションするノード数)の数だけメモリを確保
	for (int i = 0; i < m_NumChannels; ++i)
	{
		m_Channels[i].NodeName = panim->mChannels[i]->mNodeName.data;								// 読み込んだデータから名前を抽出
		m_Channels[i].nNumPositionKeys = panim->mChannels[i]->mNumPositionKeys;						// 読み込んだデータから座標キー数を抽出
		m_Channels[i].nNumRotateKeys = panim->mChannels[i]->mNumRotationKeys;						// 読み込んだデータから回転キー数を抽出
		m_Channels[i].nNumScaleKeys = panim->mChannels[i]->mNumScalingKeys;							// 読み込んだデータから拡縮キー数を抽出

		m_Channels[i].PositionKeys = new VectorKey[m_Channels[i].nNumPositionKeys];					// 位置キーの数だけキーデータを抽出するためにメモリを確保
		m_Channels[i].RotateKeys = new QuaternionKey[m_Channels[i].nNumRotateKeys];					// 回転キーの数だけキーデータを抽出するためにメモリを確保
		m_Channels[i].ScaleKeys = new VectorKey[m_Channels[i].nNumScaleKeys];						// 拡縮キーの数だけキーデータを抽出するためにメモリを確保

		// ---読み込んだデータから座標キーデータを抽出---
		for (int j = 0; j < m_Channels[i].nNumPositionKeys; ++j)
		{
			m_Channels[i].PositionKeys[j].Value.x = panim->mChannels[i]->mPositionKeys[j].mValue.x;
			m_Channels[i].PositionKeys[j].Value.y = panim->mChannels[i]->mPositionKeys[j].mValue.y;
			m_Channels[i].PositionKeys[j].Value.z = panim->mChannels[i]->mPositionKeys[j].mValue.z;

			m_Channels[i].PositionKeys[j].Time = panim->mChannels[i]->mPositionKeys[j].mTime;
		}

		// ---読み込んだデータから回転キーデータを抽出(クォータニオン)---
		for (int j = 0; j < m_Channels[i].nNumRotateKeys; ++j)
		{
			m_Channels[i].RotateKeys[j].Value.x = panim->mChannels[i]->mRotationKeys[j].mValue.x;
			m_Channels[i].RotateKeys[j].Value.y = panim->mChannels[i]->mRotationKeys[j].mValue.y;
			m_Channels[i].RotateKeys[j].Value.z = panim->mChannels[i]->mRotationKeys[j].mValue.z;
			m_Channels[i].RotateKeys[j].Value.w = panim->mChannels[i]->mRotationKeys[j].mValue.w;

			m_Channels[i].RotateKeys[j].Time = panim->mChannels[i]->mRotationKeys[j].mTime;
		}
		// ---読み込んだデータから拡縮キーデータを抽出---
		for (int j = 0; j < m_Channels[i].nNumScaleKeys; ++j)
		{
			m_Channels[i].ScaleKeys[j].Value.x = panim->mChannels[i]->mScalingKeys[j].mValue.x;
			m_Channels[i].ScaleKeys[j].Value.y = panim->mChannels[i]->mScalingKeys[j].mValue.y;
			m_Channels[i].ScaleKeys[j].Value.z = panim->mChannels[i]->mScalingKeys[j].mValue.z;

			m_Channels[i].ScaleKeys[j].Time = panim->mChannels[i]->mScalingKeys[j].mTime;
		}

	}
}

// ===アニメ―ション評価===
// 引数の時間から姿勢を求める
// ========================
void Animation::AnimeEvalution(float & time)
{
	float frame;
	frame = CalcPlayFrame(time);
	CalcAnime(frame);
}

// ===アニメーション時間の調整===
// fbxごとに1秒あたりの更新値が異なる場合があるので
// 一定速で更新されるようにtick値で調整 
// ==============================
float Animation::CalcPlayFrame(float & time)
{
	// 1秒あたりのティックを抽出する。指定されていない場合は既定値を想定。
	float ticksPerSecond = (m_Tick != 0.0) ? m_Tick : 25.0;		// 秒間更新単位
	// 以降の毎回の計算はティック単位で行う
	return time * ticksPerSecond / 60;
}

// ===アニメーション計算===
void Animation::CalcAnime(float & ptime)
{
	double time = 0.0f;
	if (m_Duration >= ptime) {
		time = ptime;
	}

	// --計算用変数--
	BoneMatrix BoneMat;
	XMMATRIX matrix;

	// ---チャンネルの数だけ更新---
	for (unsigned int i = 0; i < m_NumChannels; ++i) {
		AnimeInfo* channel = &m_Channels[i];
		// ---座標---
		if (channel->nNumPositionKeys > 1) {			// キーフレームが1つ以下の場合はキーの値が答えになる
			// ---現在時間がどのキーフレーム間に含まれるかを求め、比率から現在時間の姿勢を求める---
			for (int j = 1; j < channel->nNumPositionKeys; ++j)
			{
				float startFrame = channel->PositionKeys[j - 1].Time;
				float endFrame = channel->PositionKeys[j].Time;
				if (startFrame <= time && time < endFrame)
				{
					// キーの値
					const XMVECTOR startValue = XMLoadFloat3(&channel->PositionKeys[j - 1].Value);
					const XMVECTOR endValue = XMLoadFloat3(&channel->PositionKeys[j].Value);

					// 比率 = (現在地 - 開始値) / (目標値 - 開始値)
					float rate = (time - startFrame) / (endFrame - startFrame);
					XMStoreFloat3(&BoneMat.transrate, (startValue + (endValue - startValue) * rate));
					break;
				}
				else
					XMStoreFloat3(&BoneMat.transrate, XMLoadFloat3(&channel->PositionKeys[j].Value));
			}
		}

		// ---回転---
		if (channel->nNumRotateKeys > 1) {				// キーフレームが1つ以下の場合はキーの値が答えになる
			// ---現在時間がどのキーフレーム間に含まれるかを求め、比率から現在時間の姿勢を求める---
			for (int j = 1; j < channel->nNumRotateKeys; ++j)
			{
				float startFrame = channel->RotateKeys[j - 1].Time;
				float endFrame = channel->RotateKeys[j].Time;
				if (startFrame <= time && time < endFrame)
				{
					// キーの値
					const Quaternion startValue = channel->RotateKeys[j - 1].Value;
					const Quaternion endValue = channel->RotateKeys[j].Value;

					// 比率を計算する
					float rate = (time - startFrame) / (endFrame - startFrame);
					Quaternion::SLerp(&BoneMat.rotation, startValue, endValue, rate);		// 補間
					break;
				}
				else
					BoneMat.rotation = channel->RotateKeys[j].Value;
			}
		}

		// ---拡縮---
		if (channel->nNumScaleKeys > 1) {				// キーフレームが1つ以下の場合はキーの値が答えになる
			// ---現在時間がどのキーフレーム間に含まれるかを求め、比率から現在時間の姿勢を求める---
			for (int j = 1; j < channel->nNumScaleKeys; ++j)
			{
				float startFrame = channel->ScaleKeys[j - 1].Time;
				float endFrame = channel->ScaleKeys[j].Time;
				if (startFrame <= time && time < endFrame)
				{
					// キーの値
					const XMVECTOR startValue = XMLoadFloat3(&channel->ScaleKeys[j - 1].Value);
					const XMVECTOR endValue = XMLoadFloat3(&channel->ScaleKeys[j].Value);

					// 比率を計算する
					float rate = (time - startFrame) / (endFrame - startFrame);

					// 拡縮の値
					XMStoreFloat3(&BoneMat.scaling, (startValue + (endValue - startValue) * rate));
					break;
				}
				else
					BoneMat.scaling = channel->ScaleKeys[j].Value;
			}
		}

		// ---求めた値から変換マトリックスを構築---
		XMFLOAT4X4 mat;
		Quaternion::GetRotateMatrix(&mat, BoneMat.rotation);
		mat._11 *= BoneMat.scaling.x; mat._21 *= BoneMat.scaling.x; mat._31 *= BoneMat.scaling.x;
		mat._12 *= BoneMat.scaling.y; mat._22 *= BoneMat.scaling.y; mat._32 *= BoneMat.scaling.y;
		mat._13 *= BoneMat.scaling.z; mat._23 *= BoneMat.scaling.z; mat._33 *= BoneMat.scaling.z;
		mat._14 = BoneMat.transrate.x; mat._24 = BoneMat.transrate.y; mat._34 = BoneMat.transrate.z;
		matrix = XMLoadFloat4x4(&mat);
		
		// ---後からローカル姿勢だけでなく親からの姿勢も求めるため、ノードの名前をキーとして保持しておく---
		auto it = NodeMatrix.find(channel->NodeName);
		if (it != NodeMatrix.end())		// 既にノードの名前が登録済み
		{
			it->second = matrix;		// 値を更新する
		}
		else							// ノードが見つからない
		{
			NodeMatrix.insert(std::pair<std::string, XMMATRIX>(channel->NodeName, matrix));		// 値を登録する
		}

		// ---アニメーションブレンド用に分割したデータとしても保持しておく---
		auto boneit = BoneNodeMatrix.find(channel->NodeName);
		if (boneit != BoneNodeMatrix.end())
		{
			boneit->second = BoneMat;
		}
		else
		{
			BoneNodeMatrix.insert(std::pair<std::string, BoneMatrix>(channel->NodeName, BoneMat));
		}
	}

	mLastTime = time;
}

// ===ノード名からローカル姿勢行列を取得===
inline void Animation::GetLocalMatrix(XMMATRIX* matrix,std::string NodeName)
{
	// ---ノードからデータが見つかればそのデータを返す---
	auto it = NodeMatrix.find(NodeName);
	if (it != NodeMatrix.end())
	{
		*matrix = it->second;
	}

	// ---見つからなかった場合は初期値を返す---
	else
	{
		*matrix = XMMatrixIdentity();
	}
}

// ===ノード名からローカル姿勢(座標移動)を取得===
inline void Animation::GetLocalPosition(XMFLOAT3* pos, std::string NodeName)
{
	// ---ノードからデータが見つかればそのデータを返す---
	auto it = BoneNodeMatrix.find(NodeName);
	if (it != BoneNodeMatrix.end())
	{
		*pos = it->second.transrate;
	}

	// ---見つからなかった場合は初期値を返す---
	else
	{
		*pos = { 0.0f,0.0f,0.0f };
	}
}

// ===ノード名からローカル姿勢(拡縮)を取得===
inline void Animation::GetLocalScaling(XMFLOAT3* scale, std::string NodeName)
{
	// ---ノードからデータが見つかればそのデータを返す---
	auto it = BoneNodeMatrix.find(NodeName);
	if (it != BoneNodeMatrix.end())
	{
		*scale = it->second.scaling;
	}

	// ---見つからなかった場合は初期値を返す---
	else
	{
		*scale = { 1.0f,1.0f,1.0f };
	}
}

// ===ノード名からローカル姿勢(回転)を取得===
void Animation::GetLocalRotation(Quaternion* rot, std::string NodeName)
{
	// ---ノードからデータが見つかればそのデータを返す---
	auto it = BoneNodeMatrix.find(NodeName);
	if (it != BoneNodeMatrix.end())
	{
		*rot = it->second.rotation;
	}
	// ---見つからなかった場合は初期値を返す---
	else
	{
		*rot = Quaternion(0, 0, 0, 1);
	}
}

// ===アニメの総フレームを返す===
inline float& Animation::GetDuration()
{
	return m_Duration;
}

inline float& Animation::GetTick()
{
	return m_Tick;
}

// ===コンストラクタ===
Model::Model() : m_pMeshes(nullptr), m_pMaterials(nullptr), m_Root(nullptr),
				 m_MeshNum(0), m_MaterialNum(0), m_BoneNum(0),m_BoneBuffer(nullptr),m_InstancingBuffer(nullptr),m_WVP(nullptr)
{
	if (!m_sampler && m_ModelNum == 0)
	{
		CD3D11_DEFAULT def;
		CD3D11_SAMPLER_DESC sd(def);
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		GetDevice()->CreateSamplerState(&sd, &m_sampler);
	}
}

// ===デストラクタ===
Model::~Model()
{
	m_ModelNum--;				// モデル総数を減らす

	// ---メモリ解放---
	for (unsigned int i = 0; i < m_MeshNum; ++i)
	{
		delete[] m_pMeshes[i].Vertices;
		delete[] m_pMeshes[i].Indices;
		delete m_pMeshes[i].pMeshBuffer;
	}

	auto animit = m_AnimeMap.begin();
	while (animit != m_AnimeMap.end())
	{
		delete animit->second;
		++animit;
	}

	auto it = m_BoneNode.begin();
	while (it != m_BoneNode.end())
	{
		delete it->second;
		++it;
	}

	auto nodeit = m_NodeMap.begin();
	while (nodeit != m_NodeMap.end())
	{
		delete nodeit->second;
		++nodeit;
	}

	if (m_pMeshes) {
		delete[] m_pMeshes;
	}

	for (unsigned int i = 0; i < m_MaterialNum; ++i)
	{
		if (m_pMaterials[i].pTexture)
		{
			m_pMaterials[i].pTexture->Release();
		}
	}
	if (m_pMaterials) {
		delete[] m_pMaterials;
	}
	delete m_BoneBuffer;
	delete m_InstancingBuffer;
	delete m_WVP;

	if (m_ModelNum <= 0)
	{
		SAFE_RELEASE(m_sampler);
	}
}

// ===モデル読込===
bool Model::Load(const char* filename, float scale, bool flip)
{
	// assimpの設定
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_ValidateDataStructure;
	flag |= aiProcess_FlipUVs;
	flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_SplitByBoneCount;
	flag |= aiProcess_LimitBoneWeights;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_FixInfacingNormals;
	flag |= aiProcess_FindInstances;
	if (flip) flag |= aiProcess_MakeLeftHanded;
	if (flip) flag |= aiProcess_FlipWindingOrder;

	// assimpで読み込み
	const aiScene* pScene = importer.ReadFile(filename, flag);

	// ---読込失敗---
	if (!pScene)
		return false;

	m_ModelNum++;
	CreateNode(pScene->mRootNode);

	m_GlobalInverseTransform = ConvertaiMatrix(pScene->mRootNode->mTransformation);
	XMStoreFloat4x4(&m_GlobalInverseTransform, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_GlobalInverseTransform)));

	// ---aiSceneからメッシュ情報を取得---
	LoadMesh(pScene,scale);

	// ---aiSceneからマテリアル情報を取得---
	LoadMaterial(pScene, filename);

	m_BoneBuffer = new ConstantBuffer;
	m_BoneBuffer->Create(sizeof(XMFLOAT4X4) * SHADER_BONE_NUM);

	m_InstancingBuffer = new ConstantBuffer;
	m_InstancingBuffer->Create(sizeof(XMFLOAT4X4) * MAX_INSTANCING);

	m_WVP = new ConstantBuffer;
	m_WVP->Create(sizeof(XMFLOAT4X4) * 3);

	m_nCurrentAnime = 0;

	return true;
}

// ===FBXからアニメーションデータを読込===
void Model::AddAnime(const char* filename,bool flip)
{
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_ValidateDataStructure;
	flag |= aiProcess_SplitByBoneCount;
	if (flip) flag |= aiProcess_MakeLeftHanded;
	if (flip) flag |= aiProcess_FlipWindingOrder;

	// assimpで読み込み
	const aiScene* pScene = importer.ReadFile(filename, flag);
	// ---読込失敗---
	if (!pScene)
		return;

	if (!pScene->HasAnimations())
		return;

	auto it = m_AnimeMap.find(filename);
	if (it == m_AnimeMap.end())
	{
		Animation* animation = new Animation(*pScene->mAnimations);
		m_pAnimList.push_back(animation);
		m_AnimeMap.insert(std::pair<std::string, Animation*>(filename, animation));
	}
}

Model::NodeInfo* Model::CreateNode(const aiNode* node)
{
	auto it = m_NodeMap.find(node->mName.data);
	if (it == m_NodeMap.end())
	{
		NodeInfo* nodedata = new NodeInfo;

		// ---先頭ノードがない場合は先頭ノードに指定する---
		if (m_Root == nullptr)
			m_Root = nodedata;

		// ---Assimpから読み込んだデータを収集---
		nodedata->Transformation = XMLoadFloat4x4(&ConvertaiMatrix(node->mTransformation));

		if (node->mParent)
			nodedata->parent = m_NodeMap.find(node->mParent->mName.data)->second;			// 親からたどっていくので親のノードはノードマップに登録されているはず
		else
			nodedata->parent = nullptr;														// 親がnullだったらnullを格納

		nodedata->name = node->mName.data;

		m_NodeMap.insert(std::pair<std::string,NodeInfo*>(node->mName.data,nodedata));		// ノード情報を登録する

		// 末端の子ノードまで探索する
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			nodedata->Children.push_back(CreateNode(node->mChildren[i]));					// 子のノードは再帰処理で格納する
		}

		return nodedata;		// 最期の子までたどり着いたら子情報に再帰で格納するために自分を返す
	}
	else
	{
		return it->second;	// 既に登録済みだった場合はノードマップから探索した結果を返す
	}
}

// ===メッシュ読込===
void Model::LoadMesh(const aiScene* pScene,float scale)
{
	// ---メッシュ情報読込---
	m_MeshNum = pScene->mNumMeshes;
	m_pMeshes = new Mesh[m_MeshNum];

	Vertex vtx;

	for (int i = 0; i < m_MeshNum; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[i];

		// ---骨情報読込
		std::vector<std::vector<aiVertexWeight>> VertexWeights(mesh->mNumVertices);		// 頂点数分骨の重みを収集する
		m_pMeshes[i].hasBone = mesh->HasBones();
		m_pMeshes[i].name = mesh->mName.data;

		if (mesh->HasBones())
		{
			for (int j = 0; j < mesh->mNumBones; ++j)
			{
				const aiBone* bone = mesh->mBones[j];
				for (int k = 0; k < bone->mNumWeights; ++k)
					VertexWeights[bone->mWeights[k].mVertexId].push_back(aiVertexWeight(j, bone->mWeights[k].mWeight));

				if (m_BoneNode.find(bone->mName.data) == m_BoneNode.end())		// 骨情報リストに登録されていない場合
				{
					BoneInfo* boneinfo = new BoneInfo;
					boneinfo->offsetMatrix = XMLoadFloat4x4(&ConvertaiMatrix(bone->mOffsetMatrix));
					boneinfo->InverseMatrix = XMMatrixInverse(nullptr,boneinfo->offsetMatrix);
					boneinfo->FinalTransformation = XMMatrixIdentity();
					boneinfo->index = m_BoneNum;
					++m_BoneNum;

					m_BoneNode.insert(std::pair<std::string, BoneInfo*>(bone->mName.data, boneinfo));		// 骨情報リストに追加
					m_BoneList.push_back(boneinfo);
				}
			}
		}
		
		// ---頂点作成
		m_pMeshes[i].NumVertex = mesh->mNumVertices;								// 頂点数をAssimpで読込
		m_pMeshes[i].Vertices = new Model::Vertex[m_pMeshes[i].NumVertex];

		for (int j = 0; j < mesh->mNumVertices; ++j)
		{
			vtx.pos = XMFLOAT3(mesh->mVertices[j].x * scale, mesh->mVertices[j].y * scale, mesh->mVertices[j].z * scale);
			vtx.normal = mesh->HasNormals() ? XMFLOAT3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z) : XMFLOAT3(0.0f, 0.0f, 0.0f);
			vtx.uv = mesh->HasTextureCoords(0) ? XMFLOAT2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y) : XMFLOAT2(0.0f, 0.0f);
			// ---骨情報コピー
			unsigned int BoneIndecies[4] = { 0,0,0,0 };
			float BoneWeights[4] = { 0.0f,0.0f,0.0f,0.0f };
			if (mesh->HasBones())
			{
				for (int k = 0; k < VertexWeights[j].size() && k < 4; ++k)
				{
					BoneWeights[k] = VertexWeights[j][k].mWeight;
					BoneIndecies[k] = VertexWeights[j][k].mVertexId;
				}
			}
			else
				BoneWeights[0] = 1.0f;

			memcpy(vtx.weight, BoneWeights, sizeof(BoneWeights));
			memcpy(vtx.index, BoneIndecies, sizeof(BoneIndecies));

			m_pMeshes[i].Vertices[j] = vtx;								// 頂点情報作成

			m_pMeshes[i].materialID = mesh->mMaterialIndex;				// マテリアル番号割り当て
		}

		// ---インデックス作成
		m_pMeshes[i].NumIndex = mesh->mNumFaces * 3;					// 面は頂点3つで構成されるから面の数×3つ分インデックスが必要
		m_pMeshes[i].Indices = new unsigned int[m_pMeshes[i].NumIndex];
		for (int j = 0; j < mesh->mNumFaces; ++j)
		{
			aiFace face = mesh->mFaces[j];									// 面情報をAssimpから取得
			int idx = j * 3;
			m_pMeshes[i].Indices[idx + 0] = face.mIndices[0];				// 頂点番号1つめ
			m_pMeshes[i].Indices[idx + 1] = face.mIndices[1];				// 頂点番号2つめ
			m_pMeshes[i].Indices[idx + 2] = face.mIndices[2];				// 頂点番号3つめ
		}

		// メッシュを基に頂点バッファ作成
		DXBuffer::Desc desc = {};
		desc.pVtx = m_pMeshes[i].Vertices;
		desc.vtxSize = sizeof(Vertex);
		desc.vtxCount = m_pMeshes[i].NumVertex;
		desc.pIdx = m_pMeshes[i].Indices;
		desc.idxSize = sizeof(unsigned int);
		desc.idxCount = m_pMeshes[i].NumIndex;
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_pMeshes[i].pMeshBuffer = new DXBuffer;
		m_pMeshes[i].pMeshBuffer->Create(desc);
	}

	// ---骨の階層構造を作成---
	std::map<std::string, BoneInfo*>::iterator it = m_BoneNode.begin();
	while (it != m_BoneNode.end())
	{
		CreateBoneNode(pScene->mRootNode, it->first);
		++it;
	}
}

// ===マテリアル読込===
void Model::LoadMaterial(const aiScene* pScene, const char* filename)
{
	//--- マテリアルの作成
	// ファイルの探索
	std::string dir = filename;
	dir = dir.substr(0, dir.find_last_of('/') + 1);
	// マテリアル
	m_MaterialNum = pScene->mNumMaterials;
	m_pMaterials = new Material[m_MaterialNum];
	for (unsigned int i = 0; i < m_MaterialNum; ++i)
	{
		// 各種パラメーター
		aiColor3D color(0.0f, 0.0f, 0.0f);
		float shininess;
		m_pMaterials[i].diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) :
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pMaterials[i].ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) :
			DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		m_pMaterials[i].specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) :
			DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// ---テクスチャ---
		HRESULT hr;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			// そのまま探索
			ID3D11ShaderResourceView* pTex = TEX->Load(path.C_Str());

			// モデルと同じ階層を探索
			if (pTex == nullptr) {
				std::string file = dir;
				file += path.C_Str();
				pTex = TEX->Load(file.c_str());
			}

			m_pMaterials[i].pTexture = pTex;
		}
		else {
			m_pMaterials[i].pTexture = nullptr;
		}
	}
}

// ===骨のノード作成===
void Model::CreateBoneNode(const aiNode* node,std::string name)
{
	auto parent = node->FindNode(name.c_str())->mParent;		// Assimpで読み込んだノードから親の骨を探索
	if (parent != nullptr)
	{
		auto it = m_BoneNode.find(parent->mName.data);		// 骨情報リストから親の名前を探索
		if (it != m_BoneNode.end())							// 骨情報リストに親の骨情報が存在していたら親の骨にセットする
		{
			m_BoneNode[name]->parent = it->second;			// 親の骨セット
		}
		else												// NULLチェック
		{
			m_BoneNode[name]->parent = nullptr;
		}
	}
	else
		m_BoneNode[name]->parent = nullptr;

	int ChildNum = node->FindNode(name.c_str())->mNumChildren;
	for (int i = 0; i < ChildNum; ++i)
	{
		auto child = node->FindNode(name.c_str())->mChildren[i];
		if (child != nullptr)
		{
			auto it = m_BoneNode.find(child->mName.data);
			if (it != m_BoneNode.end())
			{
				m_BoneNode[name]->Children.push_back(it->second);			// 子ノードをセット
			}
		}
	}
}

void Model::UpdateNodeTransform(NodeInfo* pNode, XMMATRIX* nodematrix)
{
	std::string NodeName(pNode->name.c_str());
	m_pAnimList[m_nCurrentAnime]->GetLocalMatrix(nodematrix, NodeName);
}

void Model::CalcGlobalTransform()
{
	auto bit = m_BoneNode.begin();
	while (bit != m_BoneNode.end())
	{
		auto it = m_NodeMap.find(bit->first);
		if(it != m_NodeMap.end())
			bit->second->FinalTransformation = XMLoadFloat4x4(&m_GlobalInverseTransform) * it->second->Transformation * bit->second->offsetMatrix;
		++bit;
	}
}

// ===モデル描画===
void Model::Draw()
{
	GetDeviceContext()->PSSetSamplers(0, 1, &m_sampler);

	XMMATRIX bone = XMMatrixIdentity();					// 初期化
	XMFLOAT4X4 boneBuf[SHADER_BONE_NUM];				// 定数バッファに渡すためのワーク用

	// ---ボーンの行列を更新する---
	CalcGlobalTransform();

	for (unsigned int i = 0; i < m_MeshNum; ++i)
	{
		// メッシュに骨が結び付けられていた場合は姿勢を骨のノードからコピーする
		if (m_pMeshes[i].hasBone)
		{
			for (int j = 0; j < m_BoneList.size(); ++j)
			{
				XMStoreFloat4x4(&boneBuf[j],  m_BoneList[j]->FinalTransformation);		// 値をコピー
			}
			m_BoneBuffer->Write(boneBuf);												// 定数バッファに書き込む
			m_BoneBuffer->BindVS(3);													// 頂点シェーダ―に結び付ける
		}
		else
		{
			XMFLOAT4X4 identity[SHADER_BONE_NUM] = { XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) };
			m_BoneBuffer->Write(identity);												// 定数バッファに書き込む
			m_BoneBuffer->BindVS(3);													// 頂点シェーダ―に結び付ける
		}
		if(m_pMaterials[m_pMeshes[i].materialID].pTexture)
			SetTexturePS(m_pMaterials[m_pMeshes[i].materialID].pTexture, 0);
		m_pMeshes[i].pMeshBuffer->Draw();			// 描画
	}
}

void Model::Draw(XMFLOAT4X4 mtx)
{
	//m_pVS->Bind();							// 頂点シェーダ―セット
	//m_pPS->Bind();							// ピクセルシェーダ―セット

	GetVS(VS_LINE)->Bind();						// インスタンス描画用シェーダーセット
	GetPS(PS_TEXTURE)->Bind();
	GetDeviceContext()->PSSetSamplers(0, 1, &m_sampler);

	// 変換行列作成
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&mtx);								// ワールド変換行列
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// ビュー行列
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// プロジェクション行列

	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));

	m_WVP->Write(fMat);		// 定数バッファにセット
	m_WVP->BindVS(0);		// バインド

	// ---骨行列初期化---
	XMFLOAT4X4 boneBuf[SHADER_BONE_NUM];						// 定数バッファに渡すためのワーク用
	for (int i = 0; i < SHADER_BONE_NUM; ++i)
		boneBuf[i] = XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);		// 初期化

	for (unsigned int i = 0; i < m_MeshNum; ++i)
	{
		// メッシュに骨が結び付けられていた場合は姿勢を計算する
		if (m_pMeshes[i].hasBone)
		{
			for (int j = 0; j < m_BoneList.size(); ++j)
			{
				XMStoreFloat4x4(&boneBuf[j], m_BoneList[j]->FinalTransformation);		// 値をコピー
			}
		}
		m_BoneBuffer->Write(boneBuf);												// 定数バッファに書き込む
		m_BoneBuffer->BindVS(3);													// 頂点シェーダ―に結び付ける
		if (m_pMaterials[m_pMeshes[i].materialID].pTexture)
			SetTexturePS(m_pMaterials[m_pMeshes[i].materialID].pTexture, 0);
		m_pMeshes[i].pMeshBuffer->Draw();			// 描画
	}
}

// ===インスタンシング描画===
void Model::DrawInstancing(XMFLOAT4X4* mtx)
{
	GetVS(VS_INSTANCING)->Bind();						// インスタンス描画用シェーダーセット
	GetPS(PS_LAMBERT)->Bind();
	GetDeviceContext()->PSSetSamplers(0, 1, &m_sampler);

	m_InstancingBuffer->Write(mtx);
	m_InstancingBuffer->BindVS(2);

	for (unsigned int i = 0; i < m_MeshNum; ++i)
	{
		if (m_pMaterials[m_pMeshes[i].materialID].pTexture)
			SetTexturePS(m_pMaterials[m_pMeshes[i].materialID].pTexture, 0);
		m_pMeshes[i].pMeshBuffer->DrawInstancing();
	}
}

// ===aiMatrixからDirectXの行列に変換===
// ※aiMatrixは列優先なので注意が必要
XMFLOAT4X4 ConvertaiMatrix(const aiMatrix4x4& m)
{
	XMFLOAT4X4 mat;
	mat._11 = m.a1; mat._12 = m.a2; mat._13 = m.a3; mat._14 = m.a4;
	mat._21 = m.b1; mat._22 = m.b2; mat._23 = m.b3; mat._24 = m.b4;
	mat._31 = m.c1; mat._32 = m.c2; mat._33 = m.c3; mat._34 = m.c4;
	mat._41 = m.d1; mat._42 = m.d2; mat._43 = m.d3; mat._44 = m.d4;

	return mat;
}

// ===アニメーション再生===
// 引数:(アニメーションを評価する時間(再生中の時間),再生するアニメのインデックス
void Model::PlayAnime(float& time, int* animeindex)
{
	if (m_pAnimList.size() <= 0)
		return;
	if (*animeindex >= m_pAnimList.size())					// 指定したインデックスが総アニメ―ション数を超えている場合は計算しない
	{
		*animeindex = m_nCurrentAnime;
		return;
	}
	m_nCurrentAnime = *animeindex;				// 再生するアニメのインデックスを更新

	// ---現在アニメーションを更新---
	m_pAnimList[m_nCurrentAnime]->AnimeEvalution(time);		// 現在アニメーションを時間から評価

	// ---現在アニメーションで姿勢を更新
	UpdateTransforms(m_Root, m_nCurrentAnime);			// 現在アニメーションで姿勢の計算をする
}

// ===アニメーションブレンド再生===
// 引数:(アニメーションを評価する時間(再生中の時間), 再生するアニメのインデックス, (指定があれば)ブレンド再生するアニメのインデックス, (指定があれば)ブレンドの比率)
void Model::PlayBlendAnime(float& time, int* animeindex, const int& blendindex, const float& rate)
{
	if (m_pAnimList.size() <= 0)
		return;
	if (*animeindex >= m_pAnimList.size())					// 指定したインデックスが総アニメ―ション数を超えている場合は計算しない
	{
		*animeindex = m_nCurrentAnime;
		return;
	}

	m_nCurrentAnime = *animeindex;				// 再生するアニメのインデックスを更新
	m_nBlendAnime = blendindex;					// ブレンド再生するアニメのインデックスを更新

	// ---ブレンドアニメ条件---
	if (blendindex != -1)
	{
		if (rate >= 1.0f || *animeindex == blendindex) {			// ブレンドの比率が1以上の場合は完全に遷移済み
			m_nCurrentAnime = m_nBlendAnime = blendindex;		// 再生中アニメをブレンド後のインデックスと統一する
			m_nBlendAnime = -1;									// アニメーションブレンドが終わったのでブレンドアニメーションをしない状態にする
			*animeindex = m_nCurrentAnime;
		}
	}

	// ---現在アニメーションを更新---
	m_pAnimList[m_nCurrentAnime]->AnimeEvalution(time);		// 現在アニメーションを時間から評価

	// ---通常再生---
	if (m_nBlendAnime == -1)
	{
		// 骨によって決まる姿勢を更新
		UpdateTransforms(m_Root, m_nCurrentAnime);			// 現在アニメーションで姿勢の計算をする
		return;
	}

	// ---ブレンド再生---
	else if (m_nBlendAnime < m_pAnimList.size())			// 指定したインデックスが総アニメーション数を超えていたら計算ができないのでif分を抜ける
	{
		m_pAnimList[m_nBlendAnime]->AnimeEvalution(time);					// 遷移後アニメーションを評価
		UpdateTransforms(m_Root, m_nCurrentAnime, m_nBlendAnime, rate);		// ブレンド中の姿勢を計算
		return;
	}

	// ---現在アニメーションで姿勢を更新(ブレンド再生の漏れ対策)---
	UpdateTransforms(m_Root, m_nCurrentAnime);			// 現在アニメーションで姿勢の計算をする
}

// ===アニメの間隔を取得===
float Model::GetAnimDuration(int animeindex)
{
	if (animeindex > m_pAnimList.size() || m_pAnimList.size() == 0)
		return 0;
	return m_pAnimList[animeindex]->GetDuration();
}

// ===アニメのティック値を取得===
float Model::GetAnimTick(int animeindex)
{
	if (animeindex > m_pAnimList.size() || m_pAnimList.size() == 0)
		return 24.0f;
	return m_pAnimList[animeindex]->GetTick();
}

// ===特定の骨の名前の姿勢を取得する===
const XMMATRIX& Model::GetBoneMatrix(std::string bonename)
{
	auto it = m_BoneNode.find(bonename);			// 骨のノード情報から引数の名前で探索
	// ===骨情報が見つかった場合
	if (it != m_BoneNode.end())
	{
		return it->second->FinalTransformation * it->second->offsetMatrix;		// 骨の姿勢を返す
	}
	// ===骨情報が見つからなかった場合
	else
	{
		return XMMatrixIdentity();					// 行列の初期値を返す
	}
}

// ===姿勢行列を指定のアニメーションで更新(ブレンドなし)===
void Model::UpdateTransforms(NodeInfo* node, int& animeidx)
{
	// ローカル姿勢更新
	m_pAnimList[animeidx]->GetLocalMatrix(&node->LocalTransformation, node->name);

	// 全ての親マトリックスを連結して、このノードのグローバル変換マトリックスを取得
	node->Transformation = node->LocalTransformation;
	NodeInfo* parent = node->parent;
	while (parent) {
		node->Transformation = parent->LocalTransformation * node->Transformation;
		parent = parent->parent;
	}

	// ---ノードに子がある間は再帰で計算する---
	auto it = node->Children.begin();
	while (it != node->Children.end())
	{
		UpdateTransforms(*it, animeidx);
		++it;
	}
}

// ===姿勢行列を指定のアニメーションで更新(ブレンドあり)===
void Model::UpdateTransforms(NodeInfo* node, int& animeidx, int& blendidx,const float& rate)
{
	// ---変数宣言---
	XMFLOAT3 pos,blendpos;
	XMFLOAT3 scale, blendscale;
	Quaternion rot,blendrot;
	std::string name = node->name;

	// ---アニメーションリストからローカル情報を取得---
	m_pAnimList[animeidx]->GetLocalPosition(&pos, name);
	m_pAnimList[animeidx]->GetLocalScaling(&scale, name);
	m_pAnimList[animeidx]->GetLocalRotation(&rot, name);
	m_pAnimList[blendidx]->GetLocalPosition(&blendpos, name);
	m_pAnimList[blendidx]->GetLocalScaling(&blendscale, name);
	m_pAnimList[blendidx]->GetLocalRotation(&blendrot, name);

	// ---比率をかけたローカル姿勢行列を求める----
	XMFLOAT4X4 mat;
	XMMATRIX local;												// 姿勢行列を求めるためのワーク

	// ---姿勢を計算---
	pos.x = pos.x * (1.0f - rate) + blendpos.x * rate;
	pos.y = pos.y * (1.0f - rate) + blendpos.y * rate;
	pos.z = pos.z * (1.0f - rate) + blendpos.z * rate;
	scale.x = scale.x * (1.0f - rate) + blendscale.x * rate;
	scale.y = scale.y * (1.0f - rate) + blendscale.y * rate;
	scale.z = scale.z * (1.0f - rate) + blendscale.z * rate;
	Quaternion::SLerp(&rot, rot, blendrot, rate);
	Quaternion::GetRotateMatrix(&mat, rot);
	mat._11 *= scale.x; mat._21 *= scale.x; mat._31 *= scale.x;
	mat._12 *= scale.y; mat._22 *= scale.y; mat._32 *= scale.y;
	mat._13 *= scale.z; mat._23 *= scale.z; mat._33 *= scale.z;
	mat._14 = pos.x; mat._24 = pos.y; mat._34 = pos.z;

	local = XMLoadFloat4x4(&mat);
	node->LocalTransformation = local;							// ノードに格納
	node->Transformation = node->LocalTransformation;

	// ---親からの変換を求めるために親のノードがなくなるまでローカル行列をかける---
	NodeInfo* parent = node->parent;
	while (parent) {
		node->Transformation = parent->LocalTransformation * node->Transformation;
		parent = parent->parent;
	}

	// ---ノードに子がある間は再帰で計算する---
	auto it = node->Children.begin();
	while (it != node->Children.end())
	{
		UpdateTransforms(*it, animeidx, blendidx, rate);
		++it;
	}
}
// ===���f���N���X����===
// Assimp�œǂݍ��񂾃��f����`�悷��N���X�B
// Author: kimura

// ===�C���N���[�h===
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

// ===�֐��錾===
XMFLOAT4X4 ConvertaiMatrix(const aiMatrix4x4& m);

// ===�O���[�o���錾===
ID3D11SamplerState* Model::m_sampler = nullptr;
int Model::m_ModelNum = 0;

// ===�A�j���N���X�R���X�g���N�^===
Animation::Animation(const aiAnimation* panim) : mLastTime(0)
{
	LoadAnimeData(panim);
}

// ===�A�j���N���X�f�X�g���N�^===
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

// ===Assimp�œǂݍ��񂾃f�[�^����A�j���[�V�����̏���ǂݍ���===
void Animation::LoadAnimeData(const aiAnimation* panim)
{
	m_Tick = panim->mTicksPerSecond;
	m_Duration = panim->mDuration;
	m_NumChannels = panim->mNumChannels;

	m_Channels = new AnimeInfo[m_NumChannels];			// �`�����l��(�A�j���[�V��������m�[�h��)�̐��������������m��
	for (int i = 0; i < m_NumChannels; ++i)
	{
		m_Channels[i].NodeName = panim->mChannels[i]->mNodeName.data;								// �ǂݍ��񂾃f�[�^���疼�O�𒊏o
		m_Channels[i].nNumPositionKeys = panim->mChannels[i]->mNumPositionKeys;						// �ǂݍ��񂾃f�[�^������W�L�[���𒊏o
		m_Channels[i].nNumRotateKeys = panim->mChannels[i]->mNumRotationKeys;						// �ǂݍ��񂾃f�[�^�����]�L�[���𒊏o
		m_Channels[i].nNumScaleKeys = panim->mChannels[i]->mNumScalingKeys;							// �ǂݍ��񂾃f�[�^����g�k�L�[���𒊏o

		m_Channels[i].PositionKeys = new VectorKey[m_Channels[i].nNumPositionKeys];					// �ʒu�L�[�̐������L�[�f�[�^�𒊏o���邽�߂Ƀ��������m��
		m_Channels[i].RotateKeys = new QuaternionKey[m_Channels[i].nNumRotateKeys];					// ��]�L�[�̐������L�[�f�[�^�𒊏o���邽�߂Ƀ��������m��
		m_Channels[i].ScaleKeys = new VectorKey[m_Channels[i].nNumScaleKeys];						// �g�k�L�[�̐������L�[�f�[�^�𒊏o���邽�߂Ƀ��������m��

		// ---�ǂݍ��񂾃f�[�^������W�L�[�f�[�^�𒊏o---
		for (int j = 0; j < m_Channels[i].nNumPositionKeys; ++j)
		{
			m_Channels[i].PositionKeys[j].Value.x = panim->mChannels[i]->mPositionKeys[j].mValue.x;
			m_Channels[i].PositionKeys[j].Value.y = panim->mChannels[i]->mPositionKeys[j].mValue.y;
			m_Channels[i].PositionKeys[j].Value.z = panim->mChannels[i]->mPositionKeys[j].mValue.z;

			m_Channels[i].PositionKeys[j].Time = panim->mChannels[i]->mPositionKeys[j].mTime;
		}

		// ---�ǂݍ��񂾃f�[�^�����]�L�[�f�[�^�𒊏o(�N�H�[�^�j�I��)---
		for (int j = 0; j < m_Channels[i].nNumRotateKeys; ++j)
		{
			m_Channels[i].RotateKeys[j].Value.x = panim->mChannels[i]->mRotationKeys[j].mValue.x;
			m_Channels[i].RotateKeys[j].Value.y = panim->mChannels[i]->mRotationKeys[j].mValue.y;
			m_Channels[i].RotateKeys[j].Value.z = panim->mChannels[i]->mRotationKeys[j].mValue.z;
			m_Channels[i].RotateKeys[j].Value.w = panim->mChannels[i]->mRotationKeys[j].mValue.w;

			m_Channels[i].RotateKeys[j].Time = panim->mChannels[i]->mRotationKeys[j].mTime;
		}
		// ---�ǂݍ��񂾃f�[�^����g�k�L�[�f�[�^�𒊏o---
		for (int j = 0; j < m_Channels[i].nNumScaleKeys; ++j)
		{
			m_Channels[i].ScaleKeys[j].Value.x = panim->mChannels[i]->mScalingKeys[j].mValue.x;
			m_Channels[i].ScaleKeys[j].Value.y = panim->mChannels[i]->mScalingKeys[j].mValue.y;
			m_Channels[i].ScaleKeys[j].Value.z = panim->mChannels[i]->mScalingKeys[j].mValue.z;

			m_Channels[i].ScaleKeys[j].Time = panim->mChannels[i]->mScalingKeys[j].mTime;
		}

	}
}

// ===�A�j���\�V�����]��===
// �����̎��Ԃ���p�������߂�
// ========================
void Animation::AnimeEvalution(float & time)
{
	float frame;
	frame = CalcPlayFrame(time);
	CalcAnime(frame);
}

// ===�A�j���[�V�������Ԃ̒���===
// fbx���Ƃ�1�b������̍X�V�l���قȂ�ꍇ������̂�
// ��葬�ōX�V�����悤��tick�l�Œ��� 
// ==============================
float Animation::CalcPlayFrame(float & time)
{
	// 1�b������̃e�B�b�N�𒊏o����B�w�肳��Ă��Ȃ��ꍇ�͊���l��z��B
	float ticksPerSecond = (m_Tick != 0.0) ? m_Tick : 25.0;		// �b�ԍX�V�P��
	// �ȍ~�̖���̌v�Z�̓e�B�b�N�P�ʂōs��
	return time * ticksPerSecond / 60;
}

// ===�A�j���[�V�����v�Z===
void Animation::CalcAnime(float & ptime)
{
	double time = 0.0f;
	if (m_Duration >= ptime) {
		time = ptime;
	}

	// --�v�Z�p�ϐ�--
	BoneMatrix BoneMat;
	XMMATRIX matrix;

	// ---�`�����l���̐������X�V---
	for (unsigned int i = 0; i < m_NumChannels; ++i) {
		AnimeInfo* channel = &m_Channels[i];
		// ---���W---
		if (channel->nNumPositionKeys > 1) {			// �L�[�t���[����1�ȉ��̏ꍇ�̓L�[�̒l�������ɂȂ�
			// ---���ݎ��Ԃ��ǂ̃L�[�t���[���ԂɊ܂܂�邩�����߁A�䗦���猻�ݎ��Ԃ̎p�������߂�---
			for (int j = 1; j < channel->nNumPositionKeys; ++j)
			{
				float startFrame = channel->PositionKeys[j - 1].Time;
				float endFrame = channel->PositionKeys[j].Time;
				if (startFrame <= time && time < endFrame)
				{
					// �L�[�̒l
					const XMVECTOR startValue = XMLoadFloat3(&channel->PositionKeys[j - 1].Value);
					const XMVECTOR endValue = XMLoadFloat3(&channel->PositionKeys[j].Value);

					// �䗦 = (���ݒn - �J�n�l) / (�ڕW�l - �J�n�l)
					float rate = (time - startFrame) / (endFrame - startFrame);
					XMStoreFloat3(&BoneMat.transrate, (startValue + (endValue - startValue) * rate));
					break;
				}
				else
					XMStoreFloat3(&BoneMat.transrate, XMLoadFloat3(&channel->PositionKeys[j].Value));
			}
		}

		// ---��]---
		if (channel->nNumRotateKeys > 1) {				// �L�[�t���[����1�ȉ��̏ꍇ�̓L�[�̒l�������ɂȂ�
			// ---���ݎ��Ԃ��ǂ̃L�[�t���[���ԂɊ܂܂�邩�����߁A�䗦���猻�ݎ��Ԃ̎p�������߂�---
			for (int j = 1; j < channel->nNumRotateKeys; ++j)
			{
				float startFrame = channel->RotateKeys[j - 1].Time;
				float endFrame = channel->RotateKeys[j].Time;
				if (startFrame <= time && time < endFrame)
				{
					// �L�[�̒l
					const Quaternion startValue = channel->RotateKeys[j - 1].Value;
					const Quaternion endValue = channel->RotateKeys[j].Value;

					// �䗦���v�Z����
					float rate = (time - startFrame) / (endFrame - startFrame);
					Quaternion::SLerp(&BoneMat.rotation, startValue, endValue, rate);		// ���
					break;
				}
				else
					BoneMat.rotation = channel->RotateKeys[j].Value;
			}
		}

		// ---�g�k---
		if (channel->nNumScaleKeys > 1) {				// �L�[�t���[����1�ȉ��̏ꍇ�̓L�[�̒l�������ɂȂ�
			// ---���ݎ��Ԃ��ǂ̃L�[�t���[���ԂɊ܂܂�邩�����߁A�䗦���猻�ݎ��Ԃ̎p�������߂�---
			for (int j = 1; j < channel->nNumScaleKeys; ++j)
			{
				float startFrame = channel->ScaleKeys[j - 1].Time;
				float endFrame = channel->ScaleKeys[j].Time;
				if (startFrame <= time && time < endFrame)
				{
					// �L�[�̒l
					const XMVECTOR startValue = XMLoadFloat3(&channel->ScaleKeys[j - 1].Value);
					const XMVECTOR endValue = XMLoadFloat3(&channel->ScaleKeys[j].Value);

					// �䗦���v�Z����
					float rate = (time - startFrame) / (endFrame - startFrame);

					// �g�k�̒l
					XMStoreFloat3(&BoneMat.scaling, (startValue + (endValue - startValue) * rate));
					break;
				}
				else
					BoneMat.scaling = channel->ScaleKeys[j].Value;
			}
		}

		// ---���߂��l����ϊ��}�g���b�N�X���\�z---
		XMFLOAT4X4 mat;
		Quaternion::GetRotateMatrix(&mat, BoneMat.rotation);
		mat._11 *= BoneMat.scaling.x; mat._21 *= BoneMat.scaling.x; mat._31 *= BoneMat.scaling.x;
		mat._12 *= BoneMat.scaling.y; mat._22 *= BoneMat.scaling.y; mat._32 *= BoneMat.scaling.y;
		mat._13 *= BoneMat.scaling.z; mat._23 *= BoneMat.scaling.z; mat._33 *= BoneMat.scaling.z;
		mat._14 = BoneMat.transrate.x; mat._24 = BoneMat.transrate.y; mat._34 = BoneMat.transrate.z;
		matrix = XMLoadFloat4x4(&mat);
		
		// ---�ォ�烍�[�J���p�������łȂ��e����̎p�������߂邽�߁A�m�[�h�̖��O���L�[�Ƃ��ĕێ����Ă���---
		auto it = NodeMatrix.find(channel->NodeName);
		if (it != NodeMatrix.end())		// ���Ƀm�[�h�̖��O���o�^�ς�
		{
			it->second = matrix;		// �l���X�V����
		}
		else							// �m�[�h��������Ȃ�
		{
			NodeMatrix.insert(std::pair<std::string, XMMATRIX>(channel->NodeName, matrix));		// �l��o�^����
		}

		// ---�A�j���[�V�����u�����h�p�ɕ��������f�[�^�Ƃ��Ă��ێ����Ă���---
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

// ===�m�[�h�����烍�[�J���p���s����擾===
inline void Animation::GetLocalMatrix(XMMATRIX* matrix,std::string NodeName)
{
	// ---�m�[�h����f�[�^��������΂��̃f�[�^��Ԃ�---
	auto it = NodeMatrix.find(NodeName);
	if (it != NodeMatrix.end())
	{
		*matrix = it->second;
	}

	// ---������Ȃ������ꍇ�͏����l��Ԃ�---
	else
	{
		*matrix = XMMatrixIdentity();
	}
}

// ===�m�[�h�����烍�[�J���p��(���W�ړ�)���擾===
inline void Animation::GetLocalPosition(XMFLOAT3* pos, std::string NodeName)
{
	// ---�m�[�h����f�[�^��������΂��̃f�[�^��Ԃ�---
	auto it = BoneNodeMatrix.find(NodeName);
	if (it != BoneNodeMatrix.end())
	{
		*pos = it->second.transrate;
	}

	// ---������Ȃ������ꍇ�͏����l��Ԃ�---
	else
	{
		*pos = { 0.0f,0.0f,0.0f };
	}
}

// ===�m�[�h�����烍�[�J���p��(�g�k)���擾===
inline void Animation::GetLocalScaling(XMFLOAT3* scale, std::string NodeName)
{
	// ---�m�[�h����f�[�^��������΂��̃f�[�^��Ԃ�---
	auto it = BoneNodeMatrix.find(NodeName);
	if (it != BoneNodeMatrix.end())
	{
		*scale = it->second.scaling;
	}

	// ---������Ȃ������ꍇ�͏����l��Ԃ�---
	else
	{
		*scale = { 1.0f,1.0f,1.0f };
	}
}

// ===�m�[�h�����烍�[�J���p��(��])���擾===
void Animation::GetLocalRotation(Quaternion* rot, std::string NodeName)
{
	// ---�m�[�h����f�[�^��������΂��̃f�[�^��Ԃ�---
	auto it = BoneNodeMatrix.find(NodeName);
	if (it != BoneNodeMatrix.end())
	{
		*rot = it->second.rotation;
	}
	// ---������Ȃ������ꍇ�͏����l��Ԃ�---
	else
	{
		*rot = Quaternion(0, 0, 0, 1);
	}
}

// ===�A�j���̑��t���[����Ԃ�===
inline float& Animation::GetDuration()
{
	return m_Duration;
}

inline float& Animation::GetTick()
{
	return m_Tick;
}

// ===�R���X�g���N�^===
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

// ===�f�X�g���N�^===
Model::~Model()
{
	m_ModelNum--;				// ���f�����������炷

	// ---���������---
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

// ===���f���Ǎ�===
bool Model::Load(const char* filename, float scale, bool flip)
{
	// assimp�̐ݒ�
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

	// assimp�œǂݍ���
	const aiScene* pScene = importer.ReadFile(filename, flag);

	// ---�Ǎ����s---
	if (!pScene)
		return false;

	m_ModelNum++;
	CreateNode(pScene->mRootNode);

	m_GlobalInverseTransform = ConvertaiMatrix(pScene->mRootNode->mTransformation);
	XMStoreFloat4x4(&m_GlobalInverseTransform, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_GlobalInverseTransform)));

	// ---aiScene���烁�b�V�������擾---
	LoadMesh(pScene,scale);

	// ---aiScene����}�e���A�������擾---
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

// ===FBX����A�j���[�V�����f�[�^��Ǎ�===
void Model::AddAnime(const char* filename,bool flip)
{
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_ValidateDataStructure;
	flag |= aiProcess_SplitByBoneCount;
	if (flip) flag |= aiProcess_MakeLeftHanded;
	if (flip) flag |= aiProcess_FlipWindingOrder;

	// assimp�œǂݍ���
	const aiScene* pScene = importer.ReadFile(filename, flag);
	// ---�Ǎ����s---
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

		// ---�擪�m�[�h���Ȃ��ꍇ�͐擪�m�[�h�Ɏw�肷��---
		if (m_Root == nullptr)
			m_Root = nodedata;

		// ---Assimp����ǂݍ��񂾃f�[�^�����W---
		nodedata->Transformation = XMLoadFloat4x4(&ConvertaiMatrix(node->mTransformation));

		if (node->mParent)
			nodedata->parent = m_NodeMap.find(node->mParent->mName.data)->second;			// �e���炽�ǂ��Ă����̂Őe�̃m�[�h�̓m�[�h�}�b�v�ɓo�^����Ă���͂�
		else
			nodedata->parent = nullptr;														// �e��null��������null���i�[

		nodedata->name = node->mName.data;

		m_NodeMap.insert(std::pair<std::string,NodeInfo*>(node->mName.data,nodedata));		// �m�[�h����o�^����

		// ���[�̎q�m�[�h�܂ŒT������
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			nodedata->Children.push_back(CreateNode(node->mChildren[i]));					// �q�̃m�[�h�͍ċA�����Ŋi�[����
		}

		return nodedata;		// �Ŋ��̎q�܂ł��ǂ蒅������q���ɍċA�Ŋi�[���邽�߂Ɏ�����Ԃ�
	}
	else
	{
		return it->second;	// ���ɓo�^�ς݂������ꍇ�̓m�[�h�}�b�v����T���������ʂ�Ԃ�
	}
}

// ===���b�V���Ǎ�===
void Model::LoadMesh(const aiScene* pScene,float scale)
{
	// ---���b�V�����Ǎ�---
	m_MeshNum = pScene->mNumMeshes;
	m_pMeshes = new Mesh[m_MeshNum];

	Vertex vtx;

	for (int i = 0; i < m_MeshNum; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[i];

		// ---�����Ǎ�
		std::vector<std::vector<aiVertexWeight>> VertexWeights(mesh->mNumVertices);		// ���_�������̏d�݂����W����
		m_pMeshes[i].hasBone = mesh->HasBones();
		m_pMeshes[i].name = mesh->mName.data;

		if (mesh->HasBones())
		{
			for (int j = 0; j < mesh->mNumBones; ++j)
			{
				const aiBone* bone = mesh->mBones[j];
				for (int k = 0; k < bone->mNumWeights; ++k)
					VertexWeights[bone->mWeights[k].mVertexId].push_back(aiVertexWeight(j, bone->mWeights[k].mWeight));

				if (m_BoneNode.find(bone->mName.data) == m_BoneNode.end())		// ����񃊃X�g�ɓo�^����Ă��Ȃ��ꍇ
				{
					BoneInfo* boneinfo = new BoneInfo;
					boneinfo->offsetMatrix = XMLoadFloat4x4(&ConvertaiMatrix(bone->mOffsetMatrix));
					boneinfo->InverseMatrix = XMMatrixInverse(nullptr,boneinfo->offsetMatrix);
					boneinfo->FinalTransformation = XMMatrixIdentity();
					boneinfo->index = m_BoneNum;
					++m_BoneNum;

					m_BoneNode.insert(std::pair<std::string, BoneInfo*>(bone->mName.data, boneinfo));		// ����񃊃X�g�ɒǉ�
					m_BoneList.push_back(boneinfo);
				}
			}
		}
		
		// ---���_�쐬
		m_pMeshes[i].NumVertex = mesh->mNumVertices;								// ���_����Assimp�œǍ�
		m_pMeshes[i].Vertices = new Model::Vertex[m_pMeshes[i].NumVertex];

		for (int j = 0; j < mesh->mNumVertices; ++j)
		{
			vtx.pos = XMFLOAT3(mesh->mVertices[j].x * scale, mesh->mVertices[j].y * scale, mesh->mVertices[j].z * scale);
			vtx.normal = mesh->HasNormals() ? XMFLOAT3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z) : XMFLOAT3(0.0f, 0.0f, 0.0f);
			vtx.uv = mesh->HasTextureCoords(0) ? XMFLOAT2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y) : XMFLOAT2(0.0f, 0.0f);
			// ---�����R�s�[
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

			m_pMeshes[i].Vertices[j] = vtx;								// ���_���쐬

			m_pMeshes[i].materialID = mesh->mMaterialIndex;				// �}�e���A���ԍ����蓖��
		}

		// ---�C���f�b�N�X�쐬
		m_pMeshes[i].NumIndex = mesh->mNumFaces * 3;					// �ʂ͒��_3�ō\������邩��ʂ̐��~3���C���f�b�N�X���K�v
		m_pMeshes[i].Indices = new unsigned int[m_pMeshes[i].NumIndex];
		for (int j = 0; j < mesh->mNumFaces; ++j)
		{
			aiFace face = mesh->mFaces[j];									// �ʏ���Assimp����擾
			int idx = j * 3;
			m_pMeshes[i].Indices[idx + 0] = face.mIndices[0];				// ���_�ԍ�1��
			m_pMeshes[i].Indices[idx + 1] = face.mIndices[1];				// ���_�ԍ�2��
			m_pMeshes[i].Indices[idx + 2] = face.mIndices[2];				// ���_�ԍ�3��
		}

		// ���b�V������ɒ��_�o�b�t�@�쐬
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

	// ---���̊K�w�\�����쐬---
	std::map<std::string, BoneInfo*>::iterator it = m_BoneNode.begin();
	while (it != m_BoneNode.end())
	{
		CreateBoneNode(pScene->mRootNode, it->first);
		++it;
	}
}

// ===�}�e���A���Ǎ�===
void Model::LoadMaterial(const aiScene* pScene, const char* filename)
{
	//--- �}�e���A���̍쐬
	// �t�@�C���̒T��
	std::string dir = filename;
	dir = dir.substr(0, dir.find_last_of('/') + 1);
	// �}�e���A��
	m_MaterialNum = pScene->mNumMaterials;
	m_pMaterials = new Material[m_MaterialNum];
	for (unsigned int i = 0; i < m_MaterialNum; ++i)
	{
		// �e��p�����[�^�[
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

		// ---�e�N�X�`��---
		HRESULT hr;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			// ���̂܂ܒT��
			ID3D11ShaderResourceView* pTex = TEX->Load(path.C_Str());

			// ���f���Ɠ����K�w��T��
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

// ===���̃m�[�h�쐬===
void Model::CreateBoneNode(const aiNode* node,std::string name)
{
	auto parent = node->FindNode(name.c_str())->mParent;		// Assimp�œǂݍ��񂾃m�[�h����e�̍���T��
	if (parent != nullptr)
	{
		auto it = m_BoneNode.find(parent->mName.data);		// ����񃊃X�g����e�̖��O��T��
		if (it != m_BoneNode.end())							// ����񃊃X�g�ɐe�̍���񂪑��݂��Ă�����e�̍��ɃZ�b�g����
		{
			m_BoneNode[name]->parent = it->second;			// �e�̍��Z�b�g
		}
		else												// NULL�`�F�b�N
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
				m_BoneNode[name]->Children.push_back(it->second);			// �q�m�[�h���Z�b�g
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

// ===���f���`��===
void Model::Draw()
{
	GetDeviceContext()->PSSetSamplers(0, 1, &m_sampler);

	XMMATRIX bone = XMMatrixIdentity();					// ������
	XMFLOAT4X4 boneBuf[SHADER_BONE_NUM];				// �萔�o�b�t�@�ɓn�����߂̃��[�N�p

	// ---�{�[���̍s����X�V����---
	CalcGlobalTransform();

	for (unsigned int i = 0; i < m_MeshNum; ++i)
	{
		// ���b�V���ɍ������ѕt�����Ă����ꍇ�͎p�������̃m�[�h����R�s�[����
		if (m_pMeshes[i].hasBone)
		{
			for (int j = 0; j < m_BoneList.size(); ++j)
			{
				XMStoreFloat4x4(&boneBuf[j],  m_BoneList[j]->FinalTransformation);		// �l���R�s�[
			}
			m_BoneBuffer->Write(boneBuf);												// �萔�o�b�t�@�ɏ�������
			m_BoneBuffer->BindVS(3);													// ���_�V�F�[�_�\�Ɍ��ѕt����
		}
		else
		{
			XMFLOAT4X4 identity[SHADER_BONE_NUM] = { XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) };
			m_BoneBuffer->Write(identity);												// �萔�o�b�t�@�ɏ�������
			m_BoneBuffer->BindVS(3);													// ���_�V�F�[�_�\�Ɍ��ѕt����
		}
		if(m_pMaterials[m_pMeshes[i].materialID].pTexture)
			SetTexturePS(m_pMaterials[m_pMeshes[i].materialID].pTexture, 0);
		m_pMeshes[i].pMeshBuffer->Draw();			// �`��
	}
}

void Model::Draw(XMFLOAT4X4 mtx)
{
	//m_pVS->Bind();							// ���_�V�F�[�_�\�Z�b�g
	//m_pPS->Bind();							// �s�N�Z���V�F�[�_�\�Z�b�g

	GetVS(VS_LINE)->Bind();						// �C���X�^���X�`��p�V�F�[�_�[�Z�b�g
	GetPS(PS_TEXTURE)->Bind();
	GetDeviceContext()->PSSetSamplers(0, 1, &m_sampler);

	// �ϊ��s��쐬
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&mtx);								// ���[���h�ϊ��s��
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// �r���[�s��
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// �v���W�F�N�V�����s��

	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));

	m_WVP->Write(fMat);		// �萔�o�b�t�@�ɃZ�b�g
	m_WVP->BindVS(0);		// �o�C���h

	// ---���s�񏉊���---
	XMFLOAT4X4 boneBuf[SHADER_BONE_NUM];						// �萔�o�b�t�@�ɓn�����߂̃��[�N�p
	for (int i = 0; i < SHADER_BONE_NUM; ++i)
		boneBuf[i] = XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);		// ������

	for (unsigned int i = 0; i < m_MeshNum; ++i)
	{
		// ���b�V���ɍ������ѕt�����Ă����ꍇ�͎p�����v�Z����
		if (m_pMeshes[i].hasBone)
		{
			for (int j = 0; j < m_BoneList.size(); ++j)
			{
				XMStoreFloat4x4(&boneBuf[j], m_BoneList[j]->FinalTransformation);		// �l���R�s�[
			}
		}
		m_BoneBuffer->Write(boneBuf);												// �萔�o�b�t�@�ɏ�������
		m_BoneBuffer->BindVS(3);													// ���_�V�F�[�_�\�Ɍ��ѕt����
		if (m_pMaterials[m_pMeshes[i].materialID].pTexture)
			SetTexturePS(m_pMaterials[m_pMeshes[i].materialID].pTexture, 0);
		m_pMeshes[i].pMeshBuffer->Draw();			// �`��
	}
}

// ===�C���X�^���V���O�`��===
void Model::DrawInstancing(XMFLOAT4X4* mtx)
{
	GetVS(VS_INSTANCING)->Bind();						// �C���X�^���X�`��p�V�F�[�_�[�Z�b�g
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

// ===aiMatrix����DirectX�̍s��ɕϊ�===
// ��aiMatrix�͗�D��Ȃ̂Œ��ӂ��K�v
XMFLOAT4X4 ConvertaiMatrix(const aiMatrix4x4& m)
{
	XMFLOAT4X4 mat;
	mat._11 = m.a1; mat._12 = m.a2; mat._13 = m.a3; mat._14 = m.a4;
	mat._21 = m.b1; mat._22 = m.b2; mat._23 = m.b3; mat._24 = m.b4;
	mat._31 = m.c1; mat._32 = m.c2; mat._33 = m.c3; mat._34 = m.c4;
	mat._41 = m.d1; mat._42 = m.d2; mat._43 = m.d3; mat._44 = m.d4;

	return mat;
}

// ===�A�j���[�V�����Đ�===
// ����:(�A�j���[�V������]�����鎞��(�Đ����̎���),�Đ�����A�j���̃C���f�b�N�X
void Model::PlayAnime(float& time, int* animeindex)
{
	if (m_pAnimList.size() <= 0)
		return;
	if (*animeindex >= m_pAnimList.size())					// �w�肵���C���f�b�N�X�����A�j���\�V�������𒴂��Ă���ꍇ�͌v�Z���Ȃ�
	{
		*animeindex = m_nCurrentAnime;
		return;
	}
	m_nCurrentAnime = *animeindex;				// �Đ�����A�j���̃C���f�b�N�X���X�V

	// ---���݃A�j���[�V�������X�V---
	m_pAnimList[m_nCurrentAnime]->AnimeEvalution(time);		// ���݃A�j���[�V���������Ԃ���]��

	// ---���݃A�j���[�V�����Ŏp�����X�V
	UpdateTransforms(m_Root, m_nCurrentAnime);			// ���݃A�j���[�V�����Ŏp���̌v�Z������
}

// ===�A�j���[�V�����u�����h�Đ�===
// ����:(�A�j���[�V������]�����鎞��(�Đ����̎���), �Đ�����A�j���̃C���f�b�N�X, (�w�肪�����)�u�����h�Đ�����A�j���̃C���f�b�N�X, (�w�肪�����)�u�����h�̔䗦)
void Model::PlayBlendAnime(float& time, int* animeindex, const int& blendindex, const float& rate)
{
	if (m_pAnimList.size() <= 0)
		return;
	if (*animeindex >= m_pAnimList.size())					// �w�肵���C���f�b�N�X�����A�j���\�V�������𒴂��Ă���ꍇ�͌v�Z���Ȃ�
	{
		*animeindex = m_nCurrentAnime;
		return;
	}

	m_nCurrentAnime = *animeindex;				// �Đ�����A�j���̃C���f�b�N�X���X�V
	m_nBlendAnime = blendindex;					// �u�����h�Đ�����A�j���̃C���f�b�N�X���X�V

	// ---�u�����h�A�j������---
	if (blendindex != -1)
	{
		if (rate >= 1.0f || *animeindex == blendindex) {			// �u�����h�̔䗦��1�ȏ�̏ꍇ�͊��S�ɑJ�ڍς�
			m_nCurrentAnime = m_nBlendAnime = blendindex;		// �Đ����A�j�����u�����h��̃C���f�b�N�X�Ɠ��ꂷ��
			m_nBlendAnime = -1;									// �A�j���[�V�����u�����h���I������̂Ńu�����h�A�j���[�V���������Ȃ���Ԃɂ���
			*animeindex = m_nCurrentAnime;
		}
	}

	// ---���݃A�j���[�V�������X�V---
	m_pAnimList[m_nCurrentAnime]->AnimeEvalution(time);		// ���݃A�j���[�V���������Ԃ���]��

	// ---�ʏ�Đ�---
	if (m_nBlendAnime == -1)
	{
		// ���ɂ���Č��܂�p�����X�V
		UpdateTransforms(m_Root, m_nCurrentAnime);			// ���݃A�j���[�V�����Ŏp���̌v�Z������
		return;
	}

	// ---�u�����h�Đ�---
	else if (m_nBlendAnime < m_pAnimList.size())			// �w�肵���C���f�b�N�X�����A�j���[�V�������𒴂��Ă�����v�Z���ł��Ȃ��̂�if���𔲂���
	{
		m_pAnimList[m_nBlendAnime]->AnimeEvalution(time);					// �J�ڌ�A�j���[�V������]��
		UpdateTransforms(m_Root, m_nCurrentAnime, m_nBlendAnime, rate);		// �u�����h���̎p�����v�Z
		return;
	}

	// ---���݃A�j���[�V�����Ŏp�����X�V(�u�����h�Đ��̘R��΍�)---
	UpdateTransforms(m_Root, m_nCurrentAnime);			// ���݃A�j���[�V�����Ŏp���̌v�Z������
}

// ===�A�j���̊Ԋu���擾===
float Model::GetAnimDuration(int animeindex)
{
	if (animeindex > m_pAnimList.size() || m_pAnimList.size() == 0)
		return 0;
	return m_pAnimList[animeindex]->GetDuration();
}

// ===�A�j���̃e�B�b�N�l���擾===
float Model::GetAnimTick(int animeindex)
{
	if (animeindex > m_pAnimList.size() || m_pAnimList.size() == 0)
		return 24.0f;
	return m_pAnimList[animeindex]->GetTick();
}

// ===����̍��̖��O�̎p�����擾����===
const XMMATRIX& Model::GetBoneMatrix(std::string bonename)
{
	auto it = m_BoneNode.find(bonename);			// ���̃m�[�h��񂩂�����̖��O�ŒT��
	// ===����񂪌��������ꍇ
	if (it != m_BoneNode.end())
	{
		return it->second->FinalTransformation * it->second->offsetMatrix;		// ���̎p����Ԃ�
	}
	// ===����񂪌�����Ȃ������ꍇ
	else
	{
		return XMMatrixIdentity();					// �s��̏����l��Ԃ�
	}
}

// ===�p���s����w��̃A�j���[�V�����ōX�V(�u�����h�Ȃ�)===
void Model::UpdateTransforms(NodeInfo* node, int& animeidx)
{
	// ���[�J���p���X�V
	m_pAnimList[animeidx]->GetLocalMatrix(&node->LocalTransformation, node->name);

	// �S�Ă̐e�}�g���b�N�X��A�����āA���̃m�[�h�̃O���[�o���ϊ��}�g���b�N�X���擾
	node->Transformation = node->LocalTransformation;
	NodeInfo* parent = node->parent;
	while (parent) {
		node->Transformation = parent->LocalTransformation * node->Transformation;
		parent = parent->parent;
	}

	// ---�m�[�h�Ɏq������Ԃ͍ċA�Ōv�Z����---
	auto it = node->Children.begin();
	while (it != node->Children.end())
	{
		UpdateTransforms(*it, animeidx);
		++it;
	}
}

// ===�p���s����w��̃A�j���[�V�����ōX�V(�u�����h����)===
void Model::UpdateTransforms(NodeInfo* node, int& animeidx, int& blendidx,const float& rate)
{
	// ---�ϐ��錾---
	XMFLOAT3 pos,blendpos;
	XMFLOAT3 scale, blendscale;
	Quaternion rot,blendrot;
	std::string name = node->name;

	// ---�A�j���[�V�������X�g���烍�[�J�������擾---
	m_pAnimList[animeidx]->GetLocalPosition(&pos, name);
	m_pAnimList[animeidx]->GetLocalScaling(&scale, name);
	m_pAnimList[animeidx]->GetLocalRotation(&rot, name);
	m_pAnimList[blendidx]->GetLocalPosition(&blendpos, name);
	m_pAnimList[blendidx]->GetLocalScaling(&blendscale, name);
	m_pAnimList[blendidx]->GetLocalRotation(&blendrot, name);

	// ---�䗦�����������[�J���p���s������߂�----
	XMFLOAT4X4 mat;
	XMMATRIX local;												// �p���s������߂邽�߂̃��[�N

	// ---�p�����v�Z---
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
	node->LocalTransformation = local;							// �m�[�h�Ɋi�[
	node->Transformation = node->LocalTransformation;

	// ---�e����̕ϊ������߂邽�߂ɐe�̃m�[�h���Ȃ��Ȃ�܂Ń��[�J���s���������---
	NodeInfo* parent = node->parent;
	while (parent) {
		node->Transformation = parent->LocalTransformation * node->Transformation;
		parent = parent->parent;
	}

	// ---�m�[�h�Ɏq������Ԃ͍ċA�Ōv�Z����---
	auto it = node->Children.begin();
	while (it != node->Children.end())
	{
		UpdateTransforms(*it, animeidx, blendidx, rate);
		++it;
	}
}
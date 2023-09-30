// ���f��
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

#define SHADER_BONE_NUM (64)			// �V�F�[�_�[�ɓn���ő卜��
#define MAX_INSTANCING (100)			// �C���X�^���V���O�`��ő吔

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
		std::string NodeName;			// �A�j���[�V��������m�[�h�̖��O
		int nNumPositionKeys;			// �ʒu�A�j���[�V�����̃L�[�̐�
		VectorKey* PositionKeys;		// �ʒu�L�[�̃f�[�^
		int nNumRotateKeys;				// ��]�A�j���[�V�����̃L�[�̐�
		QuaternionKey* RotateKeys;		// ��]�L�[�̃f�[�^
		int nNumScaleKeys;				// �g�k�L�[�̐�
		VectorKey* ScaleKeys;			// �g�k�L�[�f�[�^
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
	std::map<std::string, XMMATRIX> NodeMatrix;			// ���[�J���̎p�����m�[�h������Z�o����
	std::map<std::string, BoneMatrix> BoneNodeMatrix;		// ���[�J���̎p��(�A�j���[�V�����u�����h�p�ɕ�����������)

	std::vector<XMFLOAT4X4> m_Transforms;
	std::vector<BoneMatrix> m_Brends;
	float mLastTime;										// �Ŋ��̃A�j���X�V����

	float m_Tick;
	float m_Duration;

	int m_NumChannels;
	AnimeInfo* m_Channels;									// �A�j���[�V������񃊃X�g
};

class Model
{
public:
	struct Vertex {
		XMFLOAT3 pos;				// ���_���W
		XMFLOAT3 normal;			// �@��
		XMFLOAT2 uv;				// �e�N�X�`�����W
		// �{�[���̌��т��A�e��
		float weight[4];			// �{�[���ɂ��e��
		unsigned int index[4];		// �ǂ̍��Ɍ��ѕt����
	};

	struct Material {
		XMFLOAT4 diffuse;						// �g�U��
		XMFLOAT4 ambient;						// ����
		XMFLOAT4 specular;						// ���ʔ���
		ID3D11ShaderResourceView* pTexture;		// �e�N�X�`��
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
		XMMATRIX offsetMatrix;					// �I�t�Z�b�g�s��(�t�o�C���h�|�[�Y���Ƃ邽�߂̍s��)
		XMMATRIX InverseMatrix;					// �I�t�Z�b�g�s���ł������s��
		XMMATRIX FinalTransformation;			// �ŏI�I�Ɍv�Z���ꂽ���̍s��
		int index;								// ���ԍ�
		BoneInfo* parent;						// �e�̍�
		std::vector<BoneInfo*> Children;		// �q�̍�
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

private:			// ===���J���Ȃ��֐�

	void LoadMesh(const aiScene* pScene, float scale = 1.0f);
	void LoadMaterial(const aiScene* pScene, const char* filename);

	void CreateBoneNode(const aiNode* node, std::string name);
	NodeInfo* CreateNode(const aiNode* node);


	void UpdateNodeTransform(NodeInfo* pNode,XMMATRIX* nodematrix);
	void CalcGlobalTransform();			// ���[�g�m�[�h����H��A�S�Ă̍��̍ŏI�I�ȕϊ��s������߂�

	void UpdateTransforms(NodeInfo* pNode,int& animeidx);							// �A�j���[�V�����X�V
	void UpdateTransforms(NodeInfo* pNode, int& animeidx, int& blendidx ,const float& rate);			// �J�ڃA�j���[�V�����X�V
protected:
	
private:
	static ID3D11SamplerState* m_sampler;				// �e�N�X�`���T���v��
	static int m_ModelNum;

	std::vector<BoneInfo*> m_BoneList;					// �����X�g
	std::map<std::string, BoneInfo*> m_BoneNode;		// ���̃m�[�h�}�b�v(���̃I�t�Z�b�g�s��A���̃C���f�b�N�X��ێ�����)
	std::map<std::string, NodeInfo*> m_NodeMap;			// Assimp�œǂݍ��񂾃m�[�h���(���f���̃��b�V���⍜�̊K�w��ێ�����)
	std::map<std::string, Animation*> m_AnimeMap;		// ���f������ǂݍ��񂾃A�j���[�V�����}�b�v(�t�@�C����)

	Mesh* m_pMeshes;									// ���b�V��
	Material* m_pMaterials;								// �}�e���A��
	int m_MeshNum;										// ���b�V���̐�
	int m_MaterialNum;									// �}�e���A���̐�
	int m_BoneNum;										// ���̐�
	ConstantBuffer* m_BoneBuffer;						// �萔�o�b�t�@(���_�V�F�[�_�\�ɓn�����̍s��Ȃ�)
	ConstantBuffer* m_InstancingBuffer;					// �萔�o�b�t�@(�C���X�^���V���O�`��p)
	ConstantBuffer* m_WVP;
	NodeInfo* m_Root;			// �m�[�h��e���炽�ǂ�K�v�����邽�ߐ擪�m�[�h�������Ă���

	std::vector<Animation*> m_pAnimList;

	XMFLOAT4X4 m_GlobalInverseTransform;
	int m_nCurrentAnime;
	int m_nBlendAnime;
};

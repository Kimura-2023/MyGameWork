#pragma once
// ===���f���Ǘ��N���X��`===
// ���f���Ǘ��N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include <vector>
#include <memory>
#include <map>
#include "Model.h"

// ===�萔�E�}�N����`===
#define MODEL (ModelManager::Get())		// �C���X�^���X�擾

// ===�w�i���f����ޒ�`===
enum MODEL_TYPE
{
	TABLE_MODEL = 0,
	CHAIR_MODEL,
	HOUSE_MODEL,
	SWORD_MODEL,
	PLAYER_MODEL,
	WALL_MODEL,
	SPIKE_MODEL,

	MAX_MODEL
};

// ===�N���X��`===
class ModelManager
{
	// ---�����o�֐�---
public:
	ModelManager();						// �R���X�g���N�^
	static ModelManager* Get();			// �C���X�^���X�擾
	static void Destroy();				// �C���X�^���X�j��
	static void Create();				// �C���X�^���X����

	void Release();						// ���f���S���

	Model* Load(std::string);			// ���f���Ǎ�
	void Draw();						// �`�揈��

	// ---�����o�ϐ�
private:
	static ModelManager* m_pInstance;									// �C���X�^���X
	std::map<std::string, std::shared_ptr<Model>> m_ModelResource;		// �ǂݍ��񂾃��f��
};

// ===�֐���`===
LPCSTR GetModelPass(MODEL_TYPE);	// imgui��Ŏg�p���郂�f���f�[�^�̃t�@�C���p�X���擾
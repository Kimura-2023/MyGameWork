// ===���f���Ǘ��N���X��`===
// ���f���Ǘ��N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include "ModelManager.h"

// ===�ÓI�����o===
ModelManager* ModelManager::m_pInstance = nullptr;

// ===���f���̃p�X===
namespace ModelKind
{
	LPCSTR pass[MAX_MODEL] = {
		"data/model/Table.fbx",
		"data/model/Chair.fbx",
		"data/model/House.fbx",
		"data/model/Sword.fbx",
		"data/model/Player.fbx",
		"data/model/Wall.fbx",
		"data/model/Spike.fbx",
	};
}

// ===�R���X�g���N�^===
ModelManager::ModelManager()
{
}

// ===����===
void ModelManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new ModelManager;
	}
}

// ===�j��===
void ModelManager::Destroy()
{
	MODEL->Release();							// ���f�����
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ===�擾===
ModelManager* ModelManager::Get()
{
	return m_pInstance;
}

// ===���f���S���===
void ModelManager::Release()
{
	auto ait = m_ModelResource.begin();				// ���f�����X�g�̐擪�C�e���[�^�擾
	while (ait != m_ModelResource.end())			// �I�[�܂ŌJ��Ԃ�
	{
		ait->second.reset();					// ���
		++ait;									// �C�e���[�^�����炷
	}
}

// ===���f���Ǎ�===
Model* ModelManager::Load(std::string file)
{
	auto it = m_ModelResource.find(file);		// �����̃p�X�ŒT��
	if (it == m_ModelResource.end())			// ���\�[�X��������Ȃ������ꍇ
	{
		std::shared_ptr<Model> pmodel = std::make_shared<Model>();
		if (pmodel->Load(file.c_str()))									// ���f�����ǂݍ��߂��ꍇ
		{
			m_ModelResource.insert(std::make_pair(file, pmodel));		// �}�b�v�ɓo�^
			return m_ModelResource.at(file).get();
		}
		else															// ���f�����ǂݍ��߂Ȃ������ꍇ
		{
			return nullptr;												// nullptr��Ԃ�
		}
	}
	else																// ���\�[�X�����������ꍇ
	{
		return it->second.get();
	}
}

// ===�C���X�^���X�`��===
void ModelManager::Draw()
{
	// ������
}

// ===�G�f�B�^��ŕ\������p�X���擾===
LPCSTR GetModelPass(MODEL_TYPE type)
{
	return ModelKind::pass[type];
}
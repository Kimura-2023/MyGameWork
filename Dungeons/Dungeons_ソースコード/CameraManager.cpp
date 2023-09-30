// ===�J�����Ǘ��N���X����===
// �J�����Ǘ��N���X������
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "CameraManager.h"
#include "DebugCamera.h"
#include "GameCamera.h"
#include "TitleCamera.h"

// ===�O���[�o���錾===
CameraManager* CameraManager::m_pInstance = nullptr;			// �C���X�^���X������

// ===�R���X�g���N�^===
CameraManager::CameraManager() : m_CurrentCamera(nullptr)
{
}

// ===�f�X�g���N�^===
CameraManager::~CameraManager()
{
	Release();			// �J�����������
}

// ===�C���X�^���X����===
void CameraManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CameraManager;		// �}�l�[�W���̃C���X�^���X����
	}
}

// ===�C���X�^���X�j��===
void CameraManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;						// �}�l�[�W���̃C���X�^���X�j��
		m_pInstance = nullptr;					// nullptr�Z�b�g
	}
}

// ===�C���X�^���X�擾===
CameraManager* CameraManager::Get()
{
	return m_pInstance;							// �C���X�^���X��Ԃ�
}

// ===�J�����Ǘ��ǉ�===
void CameraManager::RegisterCamera(std::string name,CameraType type)
{
	auto it = m_CameraList.find(name);			// �J�������X�g����J������T��
	if (it == m_CameraList.end())				// ���݂��Ȃ��ꍇ
	{
		m_CameraList.insert(std::pair<std::string, CameraBase*>(name, CreateCamera(type)));		// �J�����𐶐����A�o�^
	}
}

// ===�J�������===
void CameraManager::Release()
{
	auto it = m_CameraList.begin();				// �J�������X�g�̐擪�C�e���[�^���擾
	while (it != m_CameraList.end())			// �I�[�C�e���[�^�[�܂ŌJ��Ԃ�
	{
		delete it->second;		// �������
		++it;					// �C�e���[�^�[�����炷
	}
}

// ===�X�V===
void CameraManager::Update()
{
	// ---���݃J�����̍X�V---
	if(m_CurrentCamera)				// ���݂̃J���������݂��Ȃ��ꍇ�͉������Ȃ�
		m_CurrentCamera->Update();	// �X�V����
}

// ===�`��===
void CameraManager::Draw()
{
	if (m_CurrentCamera)			// ���݂̃J���������݂��Ȃ��ꍇ�͉������Ȃ�
		m_CurrentCamera->Clear();	// �`�揈��(�N���A)
}

// ===�J�����T������===
CameraBase* CameraManager::FindCamera(std::string name)
{
	// ---�J�����T��---
	auto it = m_CameraList.find(name);		// �J�����̃��X�g����T��
	if (it != m_CameraList.end())			// ���݂��Ă���ꍇ
	{
		return it->second;		// ���������J������Ԃ�
	}
	else									// ���݂��Ȃ��ꍇ
	{
		return nullptr;			// nullptr��Ԃ�
	}
}

// ===�J��������===
CameraBase* CameraManager::CreateCamera(CameraType type)
{
	CameraBase* pCamera;		// ���[�J��

	// ---��ʂ��番��---
	switch (type)
	{
	case GAME_CAMERA:			// ��ʂ�GAME_CAMERA�̏ꍇ
		pCamera = new GameCamera();		// �Q�[���V�[���J�����𐶐�
		return pCamera;					// ���������J������Ԃ�
		break;
	case DEBUG_CAMERA:			// ��ʂ�DEBUG_CAMERA�̏ꍇ
		pCamera = new DebugCamera();	// �f�o�b�O�V�[���J�����𐶐�
		return pCamera;					// ���������J������Ԃ�
		break;
	case TITLE_CAMERA:			// ��ʂ�TITLE_CAMERA�̏ꍇ
		pCamera = new TitleCamera();	// �^�C�g���V�[���J�����𐶐�
		return pCamera;					// ���������J������Ԃ�
		break;
	default:					// ��ʂ���`�������̈ȊO�̏ꍇ
		break;
	}
	return nullptr;		// �ǂ̒�`�ɂ��Y�����Ȃ��̂�nullptr��Ԃ�
}
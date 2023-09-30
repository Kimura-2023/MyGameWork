#pragma once

// ===�J�����Ǘ��N���X��`===
// �J�����Ǘ��N���X���`
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include <string>
#include "Camera.h"
#include <map>

// ===�萔�E�}�N����`===
#define CAMERA (CameraManager::Get())		// �C���X�^���X�擾

// ===�J������ʒ�`===
enum CameraType
{
	GAME_CAMERA,			// �Q�[���V�[���J����
	DEBUG_CAMERA,			// �f�o�b�O�V�[���J����
	TITLE_CAMERA,			// �^�C�g���V�[���J����
	NONE_CAMERA,
};

// ===�N���X��`===
class CameraManager
{
	// ---�����o�֐�---
public:
	CameraManager();					// �R���X�g���N�^
	~CameraManager();					// �f�X�g���N�^

	static CameraManager* Get();		// �C���X�^���X�擾
	static void Destroy();				// �C���X�^���X�j��
	static void Create();				// �C���X�^���X����

	void Update();						// �X�V����
	void Draw();						// �`�揈��

	void RegisterCamera(std::string name,CameraType type);				// �J�����o�^
	void SetCamera(CameraBase* camera) { m_CurrentCamera = camera; }	// ���݃J�����Z�b�g
	CameraBase* GetCurrentCamera() { return m_CurrentCamera; }			// ���݃J�����擾
	CameraBase* FindCamera(std::string);								// �J�����T��

	CameraBase* CreateCamera(CameraType);		// �J��������
	void Release();								// �J�������

	// ---�����o�ϐ�---
private:
	static CameraManager* m_pInstance;					// �C���X�^���X
	std::map<std::string, CameraBase*> m_CameraList;	// �J�����̃��X�g

	CameraBase* m_CurrentCamera;	// ���݃J����
};

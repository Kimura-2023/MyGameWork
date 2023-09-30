// ===�Q�[���Ǘ��N���X ���� GameManager.cpp
// �Q�[���Ǘ��N���X������
// ---Author kimura
// =======================================

// ===�C���N���[�h===
#include "GameManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "Fade.h"
#include "ModelManager.h"
#include <time.h>
#include "TextureManager.h"
#include "DebugProc.h"
#include "TimeController.h"
#include "ShaderList.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "SelectButtonManager.h"
#include "ScoreManager.h"

// ===�O���[�o���錾===
GameManager* GameManager::m_pInstance = nullptr;		// �C���X�^���X������

// ===�R���X�g���N�^===
GameManager::GameManager()
{
	srand(time(NULL));				// ����������
	InitShaderList();				// �g�p�V�F�[�_�[���X�g������
	TimeController::Create();		// ���ԃ}�l�[�W������
	TextureManager::Create();		// �e�N�X�`���}�l�[�W������
	ModelManager::Create();			// ���f���}�l�[�W������
	EffectManager::Create();		// �G�t�F�N�g�}�l�[�W����������
	CollisionManager::Create();		// �����蔻��}�l�[�W������
	ObjectManager::Create();		// �I�u�W�F�N�g�}�l�[�W������
	CameraManager::Create();		// �J�����}�l�[�W������
	SelectButtonManager::Create();	// �{�^���}�l�[�W������
	ScoreManager::Create();			// �X�R�A�}�l�[�W������
	SceneManager::Create();			// �V�[���}�l�[�W������
	CFade::Init();					// �t�F�[�h��ԏ�����
}

// ===�f�X�g���N�^===
GameManager::~GameManager()
{
	SceneManager::Destroy();		// �V�[���}�l�[�W���j��
	ScoreManager::Destroy();		// �X�R�A�}�l�[�W���j��
	SelectButtonManager::Destroy();	// �{�^���}�l�[�W���j��
	CameraManager::Destroy();		// �J�����}�l�[�W���j��
	ObjectManager::Destroy();		// �I�u�W�F�N�g�}�l�[�W���j��
	CollisionManager::Destroy();	// �����蔻��}�l�[�W���j��
	EffectManager::Destroy();		// �G�t�F�N�g�}�l�[�W���j��
	ModelManager::Destroy();		// ���f���}�l�[�W���j��
	TextureManager::Destroy();		// �e�N�X�`���}�l�[�W���j��
	TimeController::Destroy();		// ���ԃ}�l�[�W���j��
	UninitShaderList();				// �V�F�[�_�[���
}

// ===����===
void GameManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new GameManager;		// �C���X�^���X�𐶐�
	}
}

// ===�j��===
void GameManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// �C���X�^���X��j��
		m_pInstance = nullptr;				// null���i�[
	}
}

// ===�擾===
GameManager* GameManager::Get()
{
	return m_pInstance;						// �Q�[���}�l�[�W���̃C���X�^���X��Ԃ�
}

// ===�X�V===
void GameManager::Update()
{
	CFade::Update();						// �t�F�[�h��ԍX�V
	CAMERA->Update();						// �J�����X�V����
	TIME->Update();							// ���ԍX�V����

	// ---��~����---
	if (!TIME->GetUpdateFlg())				// ��~���Ԃ����쒆�Ȃ�X�V���Ȃ�
		return;

	// ---�}�E�X���W�X�V---
	m_Point = *CInput::GetMousePosition();					// �J�[�\���ʒu�擾
	CameraBase* pCamera = CAMERA->GetCurrentCamera();		// ���J�������擾
	if(pCamera)
	{
		// �}�E�X�J�[�\���̈ʒu�����[���h���W��(�x�N�g���Z�o)
		XMStoreFloat3(&m_MouseWorldPos, XMVector3Unproject(XMVectorSet((float)m_Point.x, (float)m_Point.y, 0.0f, 1.0f),
			GetViewPort().TopLeftX, GetViewPort().TopLeftY, GetViewPort().Width, GetViewPort().Height, GetViewPort().MinDepth, GetViewPort().MaxDepth,
			XMLoadFloat4x4(&pCamera->GetProjMatrix()), XMLoadFloat4x4(&pCamera->GetViewMatrix()),
			XMMatrixIdentity()));
	}

	// ---�Q�[���֘A�X�V---
	OBJECT->Update();						// �I�u�W�F�N�g�̍X�V
	SCENE->Update();						// �V�[���̍X�V
	COLLISION->Update();					// �����蔻��̍X�V
	EFFECT->Update();						// �G�t�F�N�g�X�V
	BUTTON->Update();						// �{�^���X�V
}

// ===�`�揈��===
void GameManager::Draw()
{
	SCENE->Draw();							// �V�[�����O�`��(�[�x�l�A�V���h�E�}�b�v)
	OBJECT->Draw();							// �I�u�W�F�N�g�`��					
	EFFECT->Draw();							// �G�t�F�N�g�`��
	COLLISION->Draw();						// �����蔻��`��(�f�o�b�O�\��)
	CFade::Draw();							// �t�F�[�h�`��
}

// ===XZ���ʏ�̃}�E�X���W(���[���h���W)�擾===
XMFLOAT3 GameManager::MousePos()
{
	// ---���[�J����`---
	XMFLOAT3 cam, mouse, vec;						// �J�������W�A�}�E�X���W�A�J�������W����}�E�X���W�ւ̃x�N�g��
	cam = CAMERA->GetCurrentCamera()->GetPos();		// ���݃J����������W�擾
	mouse = m_MouseWorldPos;						// �}�E�X���W�擾
	XMVECTOR cpos = XMLoadFloat3(&cam);				// XMVECTOR�^�ɕϊ�
	XMFLOAT3 normal = { 0.0f,1.0f,0.0f };			// ���ʂ̖@��
	XMVECTOR n = XMLoadFloat3(&normal);				// ���K��
	XMFLOAT3 floorpos = { 0.0f,0.0f,0.0f };			// ���ʍ��W

	Object* obj = SCENE->GetCurrentScene()->FindGameObject("Player");		// �v���C���[�I�u�W�F�N�g���擾
	if (obj != nullptr)
	{
		floorpos.y = obj->GetPos().y;			// ��̍��W���v���C���[��y���W�ɐݒ�
	}
	XMVECTOR f = XMLoadFloat3(&floorpos);		// XMVECTOR�^�ɕϊ�

	vec.x = mouse.x - cam.x;					// �J�������W����}�E�X���W�ւ̃x�N�g��(x����)
	vec.y = mouse.y - cam.y;					// �J�������W����}�E�X���W�ւ̃x�N�g��(y����)
	vec.z = mouse.z - cam.z;					// �J�������W����}�E�X���W�ւ̃x�N�g��(z����)
	XMVECTOR v = XMLoadFloat3(&vec);			// XMVECTOR�^�ɕϊ�
	v = DirectX::XMVector3Normalize(v);			// �x�N�g���𐳋K��
	f = f - cpos;								// �J�������W���畽�ʍ��W�ւ̃x�N�g��
	XMFLOAT3 pos;								// �i�[�p�ϐ�
	XMStoreFloat3(&pos, cpos + ((XMVector3Dot(n, f)) / (XMVector3Dot(n, v)) * v));		// �}�E�X���W(���[���h���)�v�Z

	return pos;		// ���߂����W��Ԃ�
}
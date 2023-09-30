// ===�^�C�g���V�[���N���X����===
// �^�C�g���V�[���N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "TitleScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"
#include "CameraManager.h"
#include "Sound.h"

// ===�J�����ݒ�===
struct ViewSetting
{
	DirectX::XMFLOAT3 camPos;		// ���_
	DirectX::XMFLOAT3 camLook;		// �����_
	DirectX::XMFLOAT3 camUp;		// ����x�N�g��
	char SceneName[50];				// �V�[����
};

// ===�萔��`===
#define CAMERA_FILENAME ("data/camera/setting.dat")			// �J�����p�����[�^�t�@�C����

// ===�R���X�g���N�^===
TitleScene::TitleScene()
{
	// ---������---
	m_Name = "TitleScene";			// �V�[����

	CameraManager::Get()->RegisterCamera("TitleCamera", TITLE_CAMERA);			// �J��������
	CameraBase* pCamera = CameraManager::Get()->FindCamera("TitleCamera");		// ���������J�����̃A�h���X���擾

	// ---�J�����̃p�����[�^������---
	ViewSetting setting =
	{
		pCamera->GetPos(),		// ���W
		pCamera->GetLook(),		// �����_
		pCamera->GetUp()		// ����x�N�g��
	};

	strcpy(setting.SceneName, m_Name.c_str());

	// ---�t�@�C���Ǎ�---
	FILE* fp;
	fopen_s(&fp, CAMERA_FILENAME, "rb");
	if (fp)
	{
		fread(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}
	pCamera->SetPos(setting.camPos);			// �ǂݍ��񂾃f�[�^������W��ݒ�
	pCamera->SetTarget(setting.camLook);		// �ǂݍ��񂾃f�[�^���璍���_��ݒ�
	pCamera->SetUpVector(setting.camUp);		// �ǂݍ��񂾃f�[�^�������x�N�g����ݒ�
	pCamera->UpdateMatrix();					// �ݒ肵���p�����[�^�ōX�V
	CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("TitleCamera"));		// �J�������Z�b�g

	LoadSceneData();													// �V�[���̃f�[�^��Ǎ�
	CreateGameObject("ButtonTest",UI_START_BUTTON);						// �Q�[���J�n�{�^������
	Object* button = CreateGameObject("ButtonTest2", UI_FIN_BUTTON);	// �Q�[���I���{�^������
	button->SetPos(XMFLOAT3(0.0f, -100.0f, 0.0f));						// �{�^�����W�ݒ�

	CreateGameObject("HiScore", UI_SCORE);								// �n�C�X�R�AUI����
}

// ===�f�X�g���N�^===
TitleScene::~TitleScene()
{
}

// ===�X�V����===
void TitleScene::Update()
{
}

// ===�`�揈��===
void TitleScene::Draw()
{
	SceneBase::Draw();		// �`��
}
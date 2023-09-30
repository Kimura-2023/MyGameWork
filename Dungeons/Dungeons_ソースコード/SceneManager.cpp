// ===�V�[���Ǘ��N���X ����===============
// �V�[���Ǘ��N���X������
// Author kimura
// =========================================

// ===�C���N���[�h===
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "GameScene.h"
#include "ObjectManager.h"
#include "Fade.h"
#include "GameCamera.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "ScoreManager.h"
#include "ScoreEffect.h"
#include "Sound.h"

// ===�O���[�o���錾===
SceneManager* SceneManager::m_pInstance = nullptr;		// �C���X�^���X������

// ===�R���X�g���N�^===
SceneManager::SceneManager()
{
	// ---����������---
#ifdef _DEBUG						// �f�o�b�O�r���h
	m_pScene = std::make_unique<DebugScene>();			// �f�o�b�O�V�[������
	m_eCurrentScene = SCENE_DEBUG;						// ���݃V�[�����f�o�b�O�V�[���ɃZ�b�g
	m_eChangeScene = SCENE_NONE;						// �J�ڐ�V�[������
#else								// �����[�X�r���h
	m_pScene = std::make_unique<TitleScene>();			// �^�C�g���V�[������
	m_eCurrentScene = SCENE_TITLE;						// ���݃V�[�����^�C�g���V�[���ɃZ�b�g
	m_eChangeScene = SCENE_NONE;						// �J�ڐ�V�[������
#endif
}

// ===�f�X�g���N�^===
SceneManager::~SceneManager()
{
	m_pScene.reset();			// �V�[���j��
}

// ===����===
void SceneManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneManager;	// �C���X�^���X�𐶐�
	}
}

// ===�j��===
void SceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;		// �C���X�^���X��j��
		m_pInstance = nullptr;
	}
}

// ===�擾===
SceneManager* SceneManager::Get()
{
	return m_pInstance;			// �C���X�^���X��Ԃ�
}

// ===�X�V===
void SceneManager::Update()
{
	// ---�X�V����---
	if (m_pScene != nullptr && FADE_NONE == CFade::GetState())		// �t�F�[�h���͍X�V���Ȃ�
	{
		m_pScene->Update();					// �V�[���̍X�V
	}

	// ---�V�[���؂�ւ�����---
	if (m_eChangeScene != SCENE_NONE && m_eChangeScene != m_eCurrentScene)		// �V�[���؂�ւ����N�����Ă��邩����
	{
		if (FADE_NONE == CFade::GetState())
		{
			if (m_eChangeScene != SCENE_LOAD)
				ReleaseScene();						// ���݃V�[�������

			LoadScene(m_eChangeScene);
			CFade::In(0.7f);					// �t�F�[�h����
		}
	}
}

// ===�I��===
void SceneManager::ReleaseScene()
{
	if (!m_pScene)
		return;
	m_pScene.reset();			// �V�[�����
	m_pScene = nullptr;
}

// ===�`��===
void SceneManager::Draw()
{
	// ---��O����---
	if (m_pScene == nullptr)
		return;

	// ---�J�����擾---
	CameraBase* pCamera = CAMERA->GetCurrentCamera();
	if (pCamera)
	{
		pCamera->Clear();		// �N���A
	}

	m_pScene->Draw();			// �V�[���`��(�V���h�E�}�b�v)
}

// ===�V�[���ؑ֊֐�===
// �J�ڂ���V�[�����w�肷��
void SceneManager::SetScene(Scene scene)
{
	if (m_eChangeScene != SCENE_NONE && m_eChangeScene == scene)
		return;

	m_eChangeScene = scene;			// ���V�[���������ł�������l�ɏ�������
	CFade::Out(0.7f);				// �t�F�[�h�A�E�g

	// ---�S�[���I�u�W�F�N�g�ƐڐG��---
	if (scene == SCENE_LOAD)
	{
		Object* score = m_pScene->CreateGameObject("ScoreEff", BILL_SCORE);			// �X�R�A�G�t�F�N�g�\��
		Object* goal = m_pScene->FindObjectWithTag(OT_GOAL);						// �S�[���I�u�W�F�N�g���擾
		score->SetPos(XMFLOAT3(goal->GetPos().x,goal->GetPos().y + 1.0f,goal->GetPos().z));					// �G�t�F�N�g�̍��W���Z�b�g
		((ScoreEffect*)score)->SetScore(500);										// �X�R�A���Z�b�g
		CSound::Play(SE_WARP);														// ���ʉ��Đ�
	}

}

// ===�V�[���ǂݍ���===
void SceneManager::LoadScene(Scene scene)
{
	// ---���򏈗�---
	switch (scene)					// �����̃V�[���萔���Ƃɐ�������V�[���𕪊�
	{
	case SCENE_TITLE:
		m_pScene = std::make_unique<TitleScene>();	// �^�C�g���V�[���𐶐�
		break;
	case SCENE_GAME:
		m_pScene = std::make_unique<GameScene>();	// �Q�[���V�[���𐶐�
		break;
	case SCENE_GAMEOVER:
		m_pScene = std::make_unique<GameOverScene>();	// �Q�[���I�[�o�[�V�[���𐶐�
		break;
	case SCENE_GAMECLEAR:
		m_pScene = std::make_unique<GameClearScene>();	// �Q�[���N���A�V�[���𐶐�
		break;
	case SCENE_DEBUG:
		m_pScene = std::make_unique<DebugScene>();	// �f�o�b�O�V�[���𐶐�
		break;
	case SCENE_END:
		m_pScene = std::make_unique<TitleScene>();
		break;
	case SCENE_LOAD:
		SCORE->Add(500);
		MAP->AddLevel();
		MAP->ChangeRoom(5);
		m_eChangeScene = SCENE_NONE;
		return;
	}

	m_eCurrentScene = scene;		// ���݃V�[�����X�V
	m_eChangeScene = SCENE_NONE;	// �J�ڐ�V�[�������Z�b�g
}
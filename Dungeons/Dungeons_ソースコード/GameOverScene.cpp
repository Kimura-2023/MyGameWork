// ===�Q�[���I�[�o�[�V�[���N���X����===
// �Q�[���I�[�o�[���V�[���̃N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "GameOverScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"

// ===�R���X�g���N�^===
GameOverScene::GameOverScene()
{
	m_Name = "GameOverScene";						// �V�[����
	CreateGameObject("GameOver", BG_GAMEOVER);		// �Q�[���I�[�o�[��ʃI�u�W�F�N�g����
}

// ===�f�X�g���N�^===
GameOverScene::~GameOverScene()
{
}

// ===�X�V����===
void GameOverScene::Update()
{
	// ---�L�[����---
	if (CInput::GetKeyPress(VK_RETURN) || CInput::GetJoyButton(JOYSTICKID1,VB_A))
	{
		SCENE->SetScene(SCENE_TITLE);		// �^�C�g���V�[���֑J��
	}
}

// ===�`�揈��===
void GameOverScene::Draw()
{
}
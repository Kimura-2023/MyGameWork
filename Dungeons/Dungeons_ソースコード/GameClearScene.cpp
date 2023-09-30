// ===�Q�[���N���A�V�[���N���X����===
// �Q�[���N���A���V�[���̃N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "GameClearScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"

// ===�R���X�g���N�^===
GameClearScene::GameClearScene()
{
	m_Name = "GameClearScene";						// �V�[����
	CreateGameObject("GameClear", BG_GAMECLEAR);	// �N���A��ʃI�u�W�F�N�g�𐶐�
}

// ===�f�X�g���N�^===
GameClearScene::~GameClearScene()
{
}

// ===�X�V����===
void GameClearScene::Update()
{
	// ---�L�[����---
	if (CInput::GetKeyPress(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, VB_A))
	{
		SCENE->SetScene(SCENE_TITLE);		// �^�C�g���V�[���֑J��
	}
}

// ===�`�揈��===
void GameClearScene::Draw()
{
}
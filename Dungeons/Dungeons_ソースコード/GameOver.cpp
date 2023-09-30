// ===�N���A�I�[�o�[��ʃN���X����===
// �Q�[���I�[�o�[���UI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "GameOver.h"
#include "TextureManager.h"
#include "DebugProc.h"

// ===�R���X�g���N�^===
GameOver::GameOver()
{
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �������W
	m_Scale = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);	// �T�C�Y
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ��]
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_TexCut = XMFLOAT2(1, 1);							// uv�E�����W
	m_id = BG_GAMEOVER;									// �^�O
	m_bEnable = true;									// �X�V�E�`���
	m_Texture = TEX->Load("data/texture/gameover.png");	// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
GameOver::~GameOver()
{
}

// ===�X�V����===
void GameOver::Update()
{
}

// ===�`�揈��===
void GameOver::Draw()
{
	Object2DBase::Draw();		// �`��
}

// ===�X�V�L��===
bool GameOver::IsUpdate()
{
	return true;
}
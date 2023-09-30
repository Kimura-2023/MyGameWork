// ===�N���A��ʃN���X����===
// �Q�[���N���A���UI�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "GameClear.h"
#include "TextureManager.h"
#include "DebugProc.h"

// ===�R���X�g���N�^===
GameClear::GameClear()
{
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �������W
	m_Scale = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);	// �T�C�Y
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ��]
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv������W
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_TexCut = XMFLOAT2(1, 1);							// uv�E�����W
	m_id = BG_GAMECLEAR;			// �^�O
	m_bEnable = true;				// �X�V�E�`���
	m_Texture = TEX->Load("data/texture/clear.png");	// �e�N�X�`���Ǎ�
}

// ===�f�X�g���N�^===
GameClear::~GameClear()
{
}

// ===�X�V����===
void GameClear::Update()
{
}

// ===�`�揈��===
void GameClear::Draw()
{
	Object2DBase::Draw();		// �`��
}

// ===�X�V�L��===
bool GameClear::IsUpdate()
{
	return true;
}
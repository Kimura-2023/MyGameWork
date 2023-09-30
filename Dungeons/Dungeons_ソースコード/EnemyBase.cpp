// ===�G�x�[�X�N���X����===
// �G�x�[�X�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyBase.h"
#include "EnemyMove.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "Player.h"
#include "Sound.h"
#include "SceneManager.h"
#include "EnemyAttack.h"
#include "EffectManager.h"

// ===�R���X�g���N�^===
EnemyBase::EnemyBase()
{
	// ---����������---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// �������W
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ������]
	m_transform.Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);			// �����g�k
	m_fRadius = 2.5f;										// ���E���T�C�Y
	m_bEnable = true;										// �X�V�E�`���
	m_col = true;											// �����蔻���
	m_bGravity = true;										// �d�͉�
	m_view = 5.0f;											// �v���C���[���F����
	m_ResourcePass = ("data/model/Enemy1.fbx");				// ���f���f�ނւ̃p�X
	m_Model = MODEL->Load("data/model/Enemy1.fbx");			// ���f���Ǎ�
	m_nLife = 3;											// �̗�
	m_pPS = GetPS(PS_TOON);									// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);								// ���_�V�F�[�_�[�Z�b�g
	m_State = STAY_ENEMY;									// �ҋ@��Ԃɐݒ�
	bNearFlg = false;										// �ߋ�������Z�b�g
}

// ===�f�X�g���N�^===
EnemyBase::~EnemyBase()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		delete m_pStrategy[i];
	}
}

// ===�X�V����===
void EnemyBase::Update()
{
}

// ===�`�揈��===
void EnemyBase::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);

	// �~���e�N�X�`���Z�b�g
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");
	SetTexturePS(pTex, 1);

	Object3DBase::Draw();

	// ---�֊s���`��----
	SetCullMode(CULLMODE_CW);
	m_pPS = GetPS(PS_OUTLINE);
	m_pVS = GetVS(VS_OUTLINE);
	XMFLOAT4 color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	if (bNearFlg)
	{
		SCENE->GetCurrentScene()->SetColorBuffer(&color);
	}
	Object3DBase::Draw();
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	SCENE->GetCurrentScene()->SetColorBuffer(&color);

	// ---�`���ԃ��Z�b�g---
	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);
	SetCullMode(CULLMODE_NONE);
}

// ===�X�V�ێ擾===
bool EnemyBase::IsUpdate()
{
	return true;
}

// ===��ԑJ��===
void EnemyBase::SetStrategy(ENEMY_STATE enemy)
{
	m_State = enemy;					// ��Ԃ��Z�b�g
	m_pStrategy[m_State]->Init(this);	// �������������Ă�
}
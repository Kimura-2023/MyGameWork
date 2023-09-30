// ===����N���X����===
// ����N���X������
// Author:	Kimura
//======================

// ===�C���N���[�h===
#include "Weapon.h"
#include "CollisionManager.h"
#include "DebugProc.h"
#include "TimeController.h"
#include "Sound.h"
#include "EffectManager.h"
#include "EnemyBase.h"
#include "Bullet.h"
#include "Player.h"

// ===�R���X�g���N�^===
Weapon::Weapon(Object3DBase* pobj) : Collision3D(this), m_pParent(pobj)			// �v���C���[���Z�b�g
{
	// ---������---
	m_id = PLAYER_ATACK;								// �^�O
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �������W
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ��]
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);		// �g�k
	m_fRadius = 0.5f;									// ���a
	m_bEnable = true;									// �X�V�E�`���
	m_col = false;										// �����蔻���
	m_bGravity = false;									// �d��

	AddCollisionOBB(XMFLOAT3(0.0f, 0.0f, -0.8f), XMFLOAT3(0.5f, 0.3f, 0.3f));	// �����蔻��ǉ�(OBB)
	SetCollider(true);									// �߂荞�ݖ߂��̌v�Z�Ȃ�

	m_bAttack = false;		// �U���t���O
	m_bRepel = false;		// ���˃t���O

	//COLLISION->SwitchVisibleCollision(this);
}

// ===�f�X�g���N�^===
Weapon::~Weapon()
{
}

// ===�X�V�L��===
bool Weapon::IsUpdate()
{
	return true;
}

// ===�X�V����===
void Weapon::Update()
{
	m_transform = m_pParent->GetTransform();							// �e�̏����擾
	m_vFront = m_pParent->GetFront();									// �e�̌������擾
	Object3DBase::Update();												// �X�V����

	// ---�A�j���[�V�����ɉ����ē�����---
	XMMATRIX Bone = m_pParent->GetModel()->GetBoneMatrix("hand_R");		// �{�[��(hand_R)���擾
	Bone = XMMatrixTranspose(Bone);										// �v�Z�p�v�Z
	XMFLOAT4X4 sword;													// �i�[�p
	Bone = Bone * XMLoadFloat4x4(&Object3DBase::GetWorld());			// �e�̍s��Ə�Z���A�p�������߂�
	XMStoreFloat4x4(&sword, Bone);										// �ϊ�
	Object3DBase::SetWorld(sword);										// ���߂��s������[���h�ϊ��s��Ƃ���
}

// ===�Փˎ�===
void Weapon::OnCollision(Object* obj)
{
	// ---�ʏ�U����---
	if ((obj->GetObjID() == OT_ENEMY || obj->GetObjID() == OT_ENEMY_BULLET) && m_bAttack)
	{
		if (!obj->GetInvincible())							// ���G����
		{
			obj->SetInvincible(20);							// ���G���Ԃ��Z�b�g
			TIME->SetStop(0.1f);							// �q�b�g�X�g�b�v����(0.1�b)
			CSound::Play(SE_SWORD);							// ���ʉ��Đ�
			EFFECT->Play(HIT_EFFECT, GetHitPos());			// �G�t�F�N�g�Đ�(�q�b�g�G�t�F�N�g)
			EFFECT->Play(EXPLOSION_EFFECT, GetHitPos());	// �G�t�F�N�g�Đ�(����)
			EFFECT->Play(SWORD_EFFECT, GetHitPos());		// �G�t�F�N�g�Đ�(��)
			((EnemyBase*)obj)->DelLife();					// �G���C�t����
		}
	}

	// ---�G�e���ˎ�---
	if (obj->GetObjID() == ENEMY_ATTACK && m_bRepel)
	{
		if (!obj->GetInvincible())										// ���G����
		{	
			TIME->SetStop(0.3f);										// �q�b�g�X�g�b�v����(0.3�b)
			m_pParent->SetInvincible(10);								// ���˂��Ă���Œ��̓v���C���[�ɖ��G���Ԃ𔭐�
			((Player*)m_pParent)->PlayerSetBlendAnime(ANIM_ATTACK2);	// �A�j���[�V�����J��(����U�点��)
			((Player*)m_pParent)->SetAnimeLoop(false);					// �A�j���[�V�������[�v�ݒ�
			((Player*)m_pParent)->SetAnimeRate(0.0f);					// �A�j���[�V�����J�ڗ����Z�b�g
			((Player*)m_pParent)->SetAnimeTime(0.0f);					// �A�j���[�V�������Ԃ��Z�b�g
			CSound::Play(SE_REPEL);										// ���ʉ��Đ�
			EFFECT->Play(HIT_EFFECT, GetHitPos());						// �G�t�F�N�g�Đ�(�q�b�g)
			EFFECT->Play(REPEL_EFFECT, GetHitPos());					// �G�t�F�N�g�Đ�(����)
			((Bullet*)obj)->SetRefrect(m_pParent->GetFront());			// �e�������Ă�������ɒ��˕Ԃ�
		}
	}
}

// ===���\�[�X�Z�b�g===
void Weapon::SetResource(std::string)
{
}

// ===���˃t���O�Z�b�g===
void Weapon::SetRepel(const bool& flg)
{
	m_bRepel = flg;
	m_bAttack = !flg;
}

// ===�ʏ�U���t���O�Z�b�g===
void Weapon::SetAttack(const bool& flg)
{
	m_bAttack = flg;
	m_bRepel = !flg;
}
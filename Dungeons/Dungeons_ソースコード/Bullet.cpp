// ===�e�N���X����===
// �G�̒e�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Bullet.h"
#include "CollisionManager.h"
#include "DebugProc.h"
#include "TimeController.h"
#include "Sound.h"
#include "EffectManager.h"
#include "EnemyBase.h"
#include "ModelManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "Player.h"

// ===�R���X�g���N�^===
Bullet::Bullet() : Collision3D(this)
{
	m_id = ENEMY_ATTACK;
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_fRadius = 0.5f;
	m_bEnable = false;
	m_col = false;
	m_bGravity = false;
	AddCollisionOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.8f));
	SetCollider(true);															// �����蔻��͎�邪�A�߂��̌v�Z�͂��Ȃ�

	m_ResourcePass = "data/model/Bullet.fbx";
	m_Model = MODEL->Load("data/model/Bullet.fbx");

	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);

	m_bRefrect = false;
}

// ===�f�X�g���N�^===
Bullet::~Bullet()
{
}

// ===�X�V�ێ擾===
bool Bullet::IsUpdate()
{
	return true;
}

// ===�X�V����===
void Bullet::Update()
{
	Object3DBase::Update();				// �I�u�W�F�N�g�X�V
}

// ===�`�揈��===
void Bullet::Draw()
{
	// ---�ʏ�`��---
	SetCullMode(CULLMODE_CCW);
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");		// �~���e�N�X�`���擾
	SetTexturePS(pTex, 1);														// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();			// �`�揈��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);		// �O�ʃJ�����O�ݒ�
	m_pPS = GetPS(PS_OUTLINE);		// �֊s���s�N�Z���V�F�[�_�[���Z�b�g
	m_pVS = GetVS(VS_OUTLINE);		// �֊s�����_�V�F�[�_�[���Z�b�g
	Object3DBase::Draw();			// �`�揈��

	// ---�V�F�[�_�[�����ɖ߂�---
	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);

	SetCullMode(CULLMODE_NONE);		// �J�����O�ݒ胊�Z�b�g
}

// ===�Փˎ�����===
void Bullet::OnCollision(Object* obj)
{
	// ---�v���C���[�Փˎ�---
	if (obj->GetObjID() == OT_PLAYER && m_bRefrect == false)
	{
		if (!obj->GetInvincible())			// ���G��Ԃ𔻒f
		{
			obj->SetInvincible(30);		// ���G���Ԃ��Z�b�g
			((Player*)obj)->DelLife();	// ���C�t����
			TIME->SetStop(0.1f);		// �q�b�g�X�g�b�v����
		}
	}

	// ---�G�Փˎ�---
	if (m_bRefrect && (obj->GetObjID() == OT_ENEMY || obj->GetObjID() == OT_ENEMY_BULLET))		// �e�����˂���Ă�����
	{
		obj->SetInvincible(20);			// ���G���Ԃ��Z�b�g
		TIME->SetStop(0.2f);			// �q�b�g�X�g�b�v����
		CSound::Play(SE_SWORD);			// ���ʉ��Đ�
		EFFECT->Play(HIT_EFFECT, GetHitPos());			// �Փ˃G�t�F�N�g����
		EFFECT->Play(EXPLOSION_EFFECT, GetHitPos());	// �����G�t�F�N�g����
		((EnemyBase*)obj)->DelLife(3);	// �G���C�t����

		// ---�e������
		m_bEnable = false;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_col = false;
		m_bRefrect = false;
	}

	// ---����A�G�Փˎ�---
	if (obj->GetObjID() != PLAYER_ATACK && obj->GetObjID() != OT_ENEMY && obj->GetObjID() != OT_ENEMY_BULLET)
	{
		// ---�e������
		m_bEnable = false;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_col = false;
		m_bRefrect = false;
	}
}

// ===�e���ˏ���===
bool Bullet::Fire(const XMFLOAT3& pos, const XMFLOAT3& front, const float& speed)
{
	if (m_bEnable)			// �X�V��ԂŔ���
		return false;

	CSound::Play(SE_SHOT);	// ���ʉ��Đ�
	m_transform.Pos = pos;	// ���W���Z�b�g
	m_vFront = front;		// ���ʕ������Z�b�g

	Normalize(&m_vFront);	// ���ʕ����𐳋K��

	m_vVel.x = m_vFront.x * speed;		// �����Ă������(x����)�ɑ��x���Z�b�g
	m_vVel.y = m_vFront.y * speed;		// �����Ă������(y����)�ɑ��x���Z�b�g
	m_vVel.z = m_vFront.z * speed;		// �����Ă������(z����)�ɑ��x���Z�b�g

	m_bEnable = true;		// �X�V�A�`��������Ԃɂ���
	m_col = true;			// �����蔻����Ƃ�

	return true;
}

// ===�e���ˏ���===
void Bullet::SetRefrect(const XMFLOAT3& front)
{
	m_bRefrect = true;					// ���ˏ�Ԃ��Z�b�g

	m_vFront = front;					// �����̕����ɐ��ʕ������Z�b�g
	m_vFront.y = 0.0f;					// 

	Normalize(&m_vFront);				// ���K��

	m_vVel.x = m_vFront.x * 0.5f;		// �����Ă������(x����)�ɑ��x���Z�b�g
	m_vVel.y = m_vFront.y * 0.5f;		// �����Ă������(y����)�ɑ��x���Z�b�g
	m_vVel.z = m_vFront.z * 0.5f;		// �����Ă������(z����)�ɑ��x���Z�b�g
	
	m_bEnable = true;;		// �X�V�A�`��������Ԃɂ���
	m_col = true;	 		// �����蔻����Ƃ�
}
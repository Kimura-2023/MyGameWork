// ===�ʏ�G�N���X����===
// �ʏ�G�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyNormal.h"
#include "EnemyMove.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "Player.h"
#include "TimeController.h"
#include "Sound.h"
#include "SceneManager.h"
#include "EnemyAttack.h"
#include "EffectManager.h"
#include "EnemyStay.h"
#include "ScoreManager.h"
#include "ScoreEffect.h"
#include "CameraManager.h"
#include "GameCamera.h"

// ===�R���X�g���N�^===
EnemyNormal::EnemyNormal() : Collision3D(this)
{
	// ---����������---
	m_id = OT_ENEMY;											// �I�u�W�F�N�g�̌^��ݒ�
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);				// �������W
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);				// ������]
	m_transform.Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);				// �����g�k
	m_fRadius = 2.5f;											// ���E���T�C�Y
	m_bEnable = true;											// �X�V�E�`���
	m_col = true;												// �����蔻���
	m_bGravity = true;											// �d�͉�
	m_view = 5.0f;												// �v���C���[���F����
	m_ResourcePass = ("data/model/Enemy1.fbx");					// ���f���f�ނւ̃p�X
	m_Model = MODEL->Load("data/model/Enemy1.fbx");				// ���f���Ǎ�
	m_nLife = 3;												// �̗�
	AddCollisionSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);		// �����蔻��ǉ�
	Init();														// ����������

	m_pPS = GetPS(PS_TOON);										// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);									// ���_�V�F�[�_�[�Z�b�g
	m_State = MOVE_ENEMY;										// �ҋ@��Ԃɐݒ�
	bNearFlg = false;											// �ߋ�������Z�b�g
}

// ===����������===
void EnemyNormal::Init()
{
	m_pStrategy[STAY_ENEMY] = new EnemyStay;			// �ҋ@��Ԑ���
	m_pStrategy[MOVE_ENEMY] = new EnemyMove;			// �ړ���Ԑ���
	m_pStrategy[ATTACK_ENEMY] = new EnemyAttack;		// �U����Ԑ���
}

// ===�f�X�g���N�^===
EnemyNormal::~EnemyNormal()
{
}

// ===�X�V����===
void EnemyNormal::Update()
{
	// ---���W�␳---
	if (m_transform.Pos.y <= -5.0f)
		m_transform.Pos.y = 1.0f;

	// ---�X�V����---
	if (!m_bEnable)
		return;

	// ---���S����---
	if (m_nLife <= 0)
	{
		m_bEnable = false;												// �X�V�E�`��ۏ�Ԃ��Z�b�g
		CSound::Play(SE_EXPLOSION);										// ���ʉ��Đ�
		EFFECT->Play(ENEMY_DEAD_EFFECT,m_transform.Pos);				// �G�t�F�N�g�Đ�
		m_AttackInterval = 0.0f;										// �U���Ԋu���Z�b�g
		((GameCamera*)CAMERA->GetCurrentCamera())->Shake(0.3f);			// �J�����U��

		Object* score = SCENE->GetCurrentScene()->CreateGameObject("ScoreEff", BILL_SCORE);			// �X�R�A�G�t�F�N�g����
		score->SetPos(XMFLOAT3(m_transform.Pos.x, m_transform.Pos.y + 1.0f, m_transform.Pos.z));						// ���W���Z�b�g
		score->UpdateMatrix();											// �s��X�V
		((ScoreEffect*)score)->SetScore(10);							// �X�R�A��ݒ�
		SCORE->Add(10);													// �X�R�A�X�V
		m_nLife = 3;													// ���C�t������
		return;
	}

	// ---�U���Ԋu����---
	if (m_AttackInterval > 0.0f)
		--m_AttackInterval;					// �U���Ԋu���Ԍ���
	else
		m_AttackInterval = 0.0f;			// �ŏ��l

	// ---�e�X�V����---
	m_pStrategy[m_State]->Update(this);		// ��Ԃ��Ƃ̍X�V
	m_transform.Rot.z += 0.1f;				// ���X�ɉ�]
	Object3DBase::Update();					// �I�u�W�F�N�g�X�V
	m_bGravity = true;						// �d�͗L��
}

// ===�`�揈��===
void EnemyNormal::Draw()
{
	EnemyBase::Draw();						// �`��
}

// ===�Փˏ���===
void EnemyNormal::OnCollision(Object* obj)
{
	// ---�v���C���[�ƏՓ�---
	if (obj->GetObjID() == OT_PLAYER)
	{
		if (!obj->GetInvincible() && m_State == ATTACK_ENEMY)			// �U����Ԃ̏ꍇ
		{
			((Player*)obj)->DelLife();			// ���C�t����
		}
	}

	// ---�n�ʂƏՓ�---
	if (obj->GetObjID() == OT_GROUND)
	{
		XMFLOAT3 movement = GetMovement();		// �ړ��ʎ擾
		m_transform.Pos.y += movement.y;		// y���W�␳
		m_vAcc.y = 0.0f;
		m_vVel.y = 0.0f;		
		m_bGravity = false;						// �d�͖�����
	}

	// ---�G�ƏՓ�---
	if (obj->GetObjID() == OT_ENEMY || obj->GetObjID() == OT_PILLAR)
	{
		XMFLOAT3 movement = GetMovement();
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
	}

	// ---�ǂ܂��̓h�A�ƏՓ�----
	if (obj->GetObjID() == OT_WALL || obj->GetObjID() == OT_DOOR)
	{
		XMFLOAT3 movement = GetMovement();
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
		SetStrategy(MOVE_ENEMY);
	}
}
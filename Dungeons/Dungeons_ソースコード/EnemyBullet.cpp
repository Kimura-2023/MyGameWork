// ===�������G�N���X����===
// �������G�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyBullet.h"
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
#include "BulletEnemyAttack.h"
#include "EnemyStay.h"
#include "CollisionManager.h"
#include "Bullet.h"
#include "ScoreManager.h"
#include "ScoreEffect.h"
#include "GameCamera.h"
#include "CameraManager.h"

// ===�R���X�g���N�^===
EnemyBullet::EnemyBullet() : Collision3D(this)
{
	// ---����������---
	m_id = OT_ENEMY_BULLET;									// �I�u�W�F�N�g�̌^��ݒ�
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// �������W
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ������]
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);			// �����g�k
	m_fRadius = 2.5f;										// ���E���T�C�Y
	m_bEnable = true;										// �X�V�E�`���
	m_col = true;											// �����蔻���
	m_bGravity = true;										// �d�͉�
	m_view = 8.0f;											// �v���C���[���F����
	m_ResourcePass = ("data/model/Enemy2.fbx");				// ���f���f�ނւ̃p�X
	m_Model = MODEL->Load("data/model/Enemy2.fbx");			// ���f���Ǎ�
	m_nLife = 3;											// �̗�
	AddCollisionOBB(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f,1.0f,1.0f));	// �����蔻��ǉ�
	Init();													// ��ԏ�����

	m_pPS = GetPS(PS_TOON);									// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_OBJECT);								// ���_�V�F�[�_�[�Z�b�g
	m_State = STAY_ENEMY;									// �ҋ@��Ԃɐݒ�
	bNearFlg = false;										// �ߋ�������Z�b�g
	m_pBullets = new Bullet[MAX_BULLET];					// �e����
}

// ===����������===
void EnemyBullet::Init()
{
	m_pStrategy[STAY_ENEMY] = new EnemyStay;				// �ҋ@��Ԑ���
	m_pStrategy[MOVE_ENEMY] = new EnemyMove;				// �ړ���Ԑ���
	m_pStrategy[ATTACK_ENEMY] = new BulletEnemyAttack;		// �U����Ԑ���
}

// ===�f�X�g���N�^===
EnemyBullet::~EnemyBullet()
{
	delete[] m_pBullets;		// �e���
}

// ===�X�V����===
void EnemyBullet::Update()
{
	if (!m_bEnable)							// �X�V����
		return;

	// ---���S����---
	if (m_nLife <= 0)
	{
		m_bEnable = false;									// �X�V���Ȃ�
		CSound::Play(SE_EXPLOSION);							// ���ʉ��Đ�
		EFFECT->Play(ENEMY_DEAD_EFFECT, m_transform.Pos);	// �G�t�F�N�g�Đ�
		m_AttackInterval = 0.0f;							// �U���^�C�}�[�����Z�b�g
		m_nLife = 3;										// ���C�t������
		Object* score = SCENE->GetCurrentScene()->CreateGameObject("ScoreEff", BILL_SCORE);							// �X�R�A�G�t�F�N�g����
		score->SetPos(XMFLOAT3(m_transform.Pos.x,m_transform.Pos.y + 1.0f,m_transform.Pos.z));						// ���W���Z�b�g
		((ScoreEffect*)score)->SetScore(20);				// �X�R�A��ݒ�
		SCORE->Add(20);		// �X�R�A�X�V
		((GameCamera*)CAMERA->GetCurrentCamera())->Shake(0.3f);			// �J�����U��
		return;
	}

	// ---�U���Ԋu�X�V---
	if (m_AttackInterval > 0.0f)
		--m_AttackInterval;			// ����
	else
		m_AttackInterval = 0.0f;	// �ŏ��l

	// ---�����ɂ���Ĕ���---
	float length = 0;				// ����
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// �v���C���[�擾
	if (player)
	{
		Length(&length, player->GetPos(), m_transform.Pos);			// �v���C���[�Ƃ̋��������߂�
		if (m_view >= length)				// ����̒l���߂������ꍇ�͍U��
		{
			SetStrategy(ATTACK_ENEMY);		// �U����ԂɑJ��
		}
		else
		{
			SetStrategy(STAY_ENEMY);		// �ҋ@��ԂɑJ��
		}
	}

	m_pStrategy[m_State]->Update(this);		// ��Ԃ��Ƃ̍X�V����
	Object3DBase::Update();					// �I�u�W�F�N�g�̍X�V
	m_bGravity = true;						// �d�͗L����
}

// ===�`�揈��===
void EnemyBullet::Draw()
{
	EnemyBase::Draw();						// �`��
}

// ===�Փˏ���===
void EnemyBullet::OnCollision(Object* obj)
{
	XMFLOAT3 movement = GetMovement();		// �ړ���

	// ---�n�ʂƏՓˎ�---
	if (obj->GetObjID() == OT_GROUND)
	{
		m_transform.Pos.y = 0.0f;
		m_vAcc.y = 0.0f;
		m_vVel.y = 0.0f;
		m_bGravity = false;
	}

	// ---���ƏՓˎ�---
	if (obj->GetObjID() == OT_PILLAR)
	{
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
	}

	// ---�������G�ƏՓˎ�---
	if (obj->GetObjID() == OT_ENEMY_BULLET)
	{
		m_transform.Pos.x += movement.x;
		m_transform.Pos.y += movement.y;
		m_transform.Pos.z += movement.z;
	}
}

// ===�e����===
void EnemyBullet::FireBullet()
{
	for (int i = 0; i < MAX_BULLET; ++i)
	{
		if (m_pBullets[i].Fire(XMFLOAT3(m_transform.Pos.x, m_transform.Pos.y + 1, m_transform.Pos.z), m_vFront, 0.3f))
			return;
	}
}
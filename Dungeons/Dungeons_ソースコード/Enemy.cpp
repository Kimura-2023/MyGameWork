// ===敵
// てきの行動を記述

#include "Enemy.h"
#include "DebugProc.h"
#include "ObjectManager.h"
#include "TextureResouce.h"
#include "EnemyMoving.h"
#include "EnemyDamage.h"
#include "Shadow.h"

// ===コンストラクタ===
Enemy::Enemy() : Collision3D(this)
{
	m_EnemyState = MOVING_ENEMY;

	m_id = OT_ENEMY;
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT2(5.0f, 5.0f);
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.Scale = XMFLOAT3(m_Scale.x, m_Scale.y, 1.0f);
	m_uv = XMFLOAT2(0.0f, 0.0f);
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_TexCut = XMFLOAT2(3, 4);

	m_nAnimCnt = 3;

	m_fdefault = 6.0f;
	m_Loop = true;
	m_bEnable = true;

	m_bBillboard = true;
	m_col = true;

	m_nLife = 1;
	AddCollisionSphere(XMFLOAT3(0, 0, 0), m_Scale.x / 2);
	Init();

	TEX->SetTex(this, TEX_ENEMY1);

	SetVel(XMFLOAT3(((rand() % 10) - 5) / 20.0f, 0, ((rand() % 10) - 5) / 20.0f));

	new Shadow(this);
}

Enemy::Enemy(XMFLOAT3 pos) : Collision3D(this)
{
	m_EnemyState = MOVING_ENEMY;

	m_id = OT_ENEMY;
	m_transform.Pos = pos;
	m_Scale = XMFLOAT2(5.0f, 5.0f);
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.Scale = XMFLOAT3(m_Scale.x, m_Scale.y, 1.0f);
	m_uv = XMFLOAT2(0.0f, 0.0f);
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_TexCut = XMFLOAT2(3, 4);

	m_nAnimCnt = 3;

	m_fdefault = 6.0f;
	m_Loop = true;
	m_bEnable = true;

	m_bBillboard = true;
	m_col = true;

	m_nLife = 1;
	AddCollisionSphere(XMFLOAT3(0, 0, 0), m_Scale.x / 2);
	Init();

	TEX->SetTex(this, TEX_ENEMY1);

	SetVel(XMFLOAT3(((rand() % 10) - 5) / 20.0f, 0, ((rand() % 10) - 5) / 20.0f));

	new Shadow(this);
}

void Enemy::Init()
{
	m_pState[MOVING_ENEMY] = new EnemyMoving;
	m_pState[DAMAGE_ENEMY] = new EnemyDamage;

	TEX->SetTex(this, TEX_ENEMY1);
}

// ===デストラクタ===
Enemy::~Enemy()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		delete m_pState[i];
	}
	Object2DBase::~Object2DBase();
}

void Enemy::Update()
{
	if (m_nLife <= 0)
	{
		OBJECT->Release(this);
		return;
	}

	oldPos = m_transform.Pos;

	Object2DBase::Update();
	Animation2DBase::Update();

	if (m_transform.Pos.y <= 0.0f + m_Scale.y / 2)
	{
		m_transform.Pos.y = m_Scale.y;
	}

	// ---移動制限---
	if (m_transform.Pos.x < -48.0f)
		m_transform.Pos.x = -48.0f;
	if (m_transform.Pos.x > 48.0f)
		m_transform.Pos.x = 48.0f;
	if (m_transform.Pos.z > 48.0f)
		m_transform.Pos.z = 48.0f;
	if (m_transform.Pos.z < -48.0f)
		m_transform.Pos.z = -48.0f;

	m_pState[m_EnemyState]->Update(this);

}

void Enemy::Draw()
{
}

bool Enemy::IsUpdate()
{
	return true;
}

void Enemy::SetState(ENEMY2D_STATE state)
{
	if (!m_pState[m_EnemyState]->IsUpdate(this))
	{
		return;
	}

	m_EnemyState = state;
}

void Enemy::OnCollision(Object* obj)
{
	if (obj->GetObjID() == OT_PLAYERATTACK)
	{
		SetState(DAMAGE_ENEMY);
		--m_nLife;
	}
}
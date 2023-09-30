// ===�G�ҋ@�N���X����===
// �G�ҋ@��ԃN���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyStay.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"
#include "EffectManager.h"

// ===����������===
void EnemyStay::Init(EnemyBase* enemy)
{
}

// ===�X�V����===
void EnemyStay::Update(EnemyBase* enemy)
{
	enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));		// ���x���Z�b�g
	enemy->SetNear(false);							// �ߋ�������Z�b�g
}
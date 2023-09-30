// ===�������G�U���N���X����===
// �������G�U���N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "BulletEnemyAttack.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"
#include "EffectManager.h"
#include "EnemyBullet.h"

// ===����������===
void BulletEnemyAttack::Init(EnemyBase* enemy)
{
	XMFLOAT3 vec;			// ����
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// �v���C���[���擾

	// ---��O����---
	if (!player)
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		enemy->SetStrategy(STAY_ENEMY);					// �ҋ@��ԂɑJ��
		return;
	}

	// ---�e��ԏ�����---
	vec.x = player->GetPos().x - enemy->GetPos().x;		// �i�s���������߂�
	vec.z = player->GetPos().z - enemy->GetPos().z;
	vec.y = 0.0f;
	Normalize(&vec);									// ���K��
	enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));			// ���x���Z�b�g
	enemy->SetFront(vec);								// ���ʕ������Z�b�g
	m_nBulletCnt = 3;									// �c�e����������
}

// ===�X�V����===
void BulletEnemyAttack::Update(EnemyBase* enemy)
{
	// ---�U���Ԋu�Ŕ���---
	if (enemy->GetInterval() <= 60.0f)
		enemy->SetNear(true);					// �U�����O�ɗ֊s���̐F��ς���
	else
		enemy->SetNear(false);					// �ʏ�ɖ߂�

	// ---�c�e���Ŕ���---
	if (m_nBulletCnt <= 0)
	{
		enemy->SetStrategy(STAY_ENEMY);			// �ҋ@��ԂɑJ��
		enemy->SetInterval(30.0f);				// �C���^�[�o�����Z�b�g
	}
	else if(enemy->GetInterval() <= 0.0f)
	{
		enemy->SetInterval(2.0f);				// �C���^�[�o�����Z�b�g
		((EnemyBullet*)enemy)->FireBullet();	// �e����
		m_nBulletCnt--;							// �c�e�������炷
	}
}
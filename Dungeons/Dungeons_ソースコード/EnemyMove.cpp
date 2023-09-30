// ===�������G�N���X����===
// �������G�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyMove.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"

// ===����������===
void EnemyMove::Init(EnemyBase* enemy)
{
}

// ===�X�V����===
void EnemyMove::Update(EnemyBase* enemy)
{
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// �v���C���[�擾

	// ---��O����---
	if (!player)
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		return;
	}

	// ---�����ɂ���ď���---
	float rad = enemy->GetView();		// ���F�����擾
	float length;						// ����
	Length(&length, player->GetPos(), enemy->GetPos());		// �v���C���[�Ƃ̋������Z�o

	if (length <= rad && length >= 2.0f)		// ���F�������̏ꍇ
	{
		XMFLOAT3 vec;		// ����
		vec.x = player->GetPos().x - enemy->GetPos().x;		// �v���C���[�ւ̌��������߂�
		vec.z = player->GetPos().z - enemy->GetPos().z;
		vec.y = 0.0f;

		Normalize(&vec);	// ���K��
		vec.x *= 0.05f;		// x�����␳
		vec.z *= 0.05f;		// z�����␳

		enemy->SetVel(vec);		// ���x���Z�b�g
		enemy->SetFront(vec);	// ���ʕ����Z�b�g
		enemy->SetNear(true);	// �ߋ�������Z�b�g
	}
	else					// ���F�����O�̏ꍇ
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		enemy->SetNear(false);
	}

	if(length <= rad && enemy->GetInterval() <= 60.0f && enemy->GetInterval() > 0.0f)
		enemy->SetNear(true);					// �ߋ�������Z�b�g
	else if (length <= 3.0f && enemy->GetInterval() <= 0.0f)		// ��苗�������U���Ԋu��0�̏ꍇ
		enemy->SetStrategy(ATTACK_ENEMY);		// �U����Ԃ֑J��
}
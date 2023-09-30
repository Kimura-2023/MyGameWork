// ===�ʏ�G�U���N���X����===
// �ʏ�G�U���N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyAttack.h"
#include "Player.h"
#include "SceneManager.h"
#include "Calc.h"
#include "EffectManager.h"
#include "Sound.h"

// ===����������===
void EnemyAttack::Init(EnemyBase* enemy)
{
	m_vStart = enemy->GetPos();					// �U���J�n�n�_���Z�b�g
	XMFLOAT3 vec;								// �i�s����
	Object* player = SCENE->GetCurrentScene()->FindGameObject("Player");		// �v���C���[�擾

	// ---��O����---
	if (!player)
	{
		enemy->SetVel(XMFLOAT3(0.0f, 0.0f, 0.0f));
		return;
	}

	// ---���x�Z�b�g---
	vec.x = player->GetPos().x - enemy->GetPos().x;
	vec.z = player->GetPos().z - enemy->GetPos().z;
	vec.y = 0.0f;

	Normalize(&vec);
	vec.x *= 0.1f;
	vec.z *= 0.1f;

	enemy->SetVel(vec);			// ���x�Z�b�g
	enemy->SetFront(vec);		// ���ʕ������Z�b�g
	enemy->SetNear(true);		// �ߋ����t���O�Z�b�g
	CSound::Play(SE_ENEMY);		// �U�����Đ�
}

// ===�X�V����===
void EnemyAttack::Update(EnemyBase* enemy)
{
	// ---���������߂�
	float length;
	Length(&length, enemy->GetPos(),m_vStart);


	EFFECT->Play(ENEMY_EFFECT, enemy->GetPos());		// �G�t�F�N�g����

	// ---���X�ɉ�]
	XMFLOAT3 rot = enemy->GetRotate();
	enemy->SetRot(XMFLOAT3(rot.x, rot.y, rot.z + 2.0f));

	// ---��苗���ȏ�i�s�������ԑJ��
	if (length >= 10.0f)
	{
		enemy->SetStrategy(MOVE_ENEMY);			// �ړ���ԂɑJ��
		enemy->SetInterval(2.0f);				// �C���^�[�o�����Z�b�g
	}

}
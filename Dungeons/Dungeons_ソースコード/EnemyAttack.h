#pragma once
// ===�ʏ�G�U���N���X��`===
// �ʏ�G�U���N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "EnemyStrategy.h"

// ---�N���X��`
class EnemyAttack : public EnemyStrategy
{
	// ---�����o�֐�
public:
	virtual void Init(EnemyBase* enemy);		// ����������
	virtual void Update(EnemyBase* enemy);		// �X�V����

	// ---�����o�ϐ�
private:
	XMFLOAT3 m_vStart;							// �U���J�n�_
};
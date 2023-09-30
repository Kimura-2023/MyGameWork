#pragma once
// ===�������G�N���X��`===
// �������G�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "EnemyStrategy.h"
#include "Collision3D.h"
#include "EnemyBase.h"
#include "Bullet.h"

// ===�萔�E�}�N����`===
#define MAX_BULLET (3)				// �ő�e��

// ===�N���X�錾===
class EnemyBullet : public EnemyBase, Collision3D
{
	// ---�����o�֐�
public:
	EnemyBullet();				// �R���X�g���N�^
	~EnemyBullet();				// �f�X�g���N�^

	void Init();					// ����������
	void Update();					// �X�V����
	void Draw();					// �`�揈��
	void OnCollision(Object*);		// �Փˏ���
	float& GetView() { return m_view; }			// ���F�����擾
	float& GetInterval() { return m_AttackInterval; }		// �U���Ԋu���Ԏ擾
	void FireBullet();			// �e����

	// ---�����o�ϐ�
private:
	Bullet* m_pBullets;			// �e
};
#pragma once
// ===�v���C���[�ړ��N���X��`===
// �v���C���[�ړ��N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStrategy.h"

// ===�N���X�錾===
class PlayerMove : public PlayerStrategy
{
	// ---�����o�֐�---
public:
	void Init(Player* player);					// ����������
	void Update(Player* player);				// �X�V����
	virtual bool IsUpdate(Player* player);		// �X�V���

	// ---�����o�ϐ�---
private:
	bool m_bController;			// �R���g���[���[���̓t���O
};
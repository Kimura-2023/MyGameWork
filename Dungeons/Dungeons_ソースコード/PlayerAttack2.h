#pragma once
// ===�v���C���[2�i�ڍU���N���X��`===
// �v���C���[2�i�ڍU���N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStrategy.h"

// ===�N���X��`===
class PlayerAttack2 : public PlayerStrategy
{
	// ---�����o�֐�---
public:
	void Init(Player* player);					// ����������
	void Update(Player* player);				// �X�V����
	virtual bool IsUpdate(Player* player);		// �X�V�L��
};
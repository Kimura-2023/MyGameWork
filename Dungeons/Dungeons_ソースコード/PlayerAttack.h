#pragma once
// ===�v���C���[�U���N���X��`===
// �v���C���[�U���N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStrategy.h"

// ===�N���X��`===
class PlayerAttack : public PlayerStrategy
{
	// ---�����o�֐�---
public:
	void Init(Player* player);					// ����������
	void Update(Player* player);				// �X�V����
	virtual bool IsUpdate(Player* player);		// �J�ڂł��邩�ǂ���
};
#pragma once
// ===�v���C���[�ҋ@�N���X��`===
// �v���C���[�ҋ@�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStrategy.h"

// ===�N���X��`===
class PlayerStay : public PlayerStrategy
{
	// ---�����o�֐�---
public:
	void Init(Player* player);					// ����������
	void Update(Player* player);				// �X�V����
	virtual bool IsUpdate(Player* player);		// �X�V��Ԏ擾
};
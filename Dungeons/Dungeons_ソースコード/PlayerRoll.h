#pragma once
// ===�v���C���[����N���X��`===
// �v���C���[����N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStrategy.h"

// ===�N���X��`===
class PlayerRoll : public PlayerStrategy
{
	// ---�����o�֐�---
public:
	void Init(Player* player);					// ����������
	void Update(Player* player);				// �X�V����
	virtual bool IsUpdate(Player* player);		// �X�V�L��
};
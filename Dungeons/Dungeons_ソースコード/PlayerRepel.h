#pragma once
// ===�v���C���[�e���N���X��`===
// �v���C���[�e���N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "PlayerStrategy.h"

// ===�N���X��`===
class PlayerRepel : public PlayerStrategy
{
	// ---�����o�֐�---
public:
	void Init(Player* player);					// ����������
	void Update(Player* player);				// �X�V����
	virtual bool IsUpdate(Player* player);		// �X�V�L��
};
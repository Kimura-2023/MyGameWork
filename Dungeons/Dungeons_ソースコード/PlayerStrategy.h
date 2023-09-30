#pragma once
// ===�v���C���[��ԃx�[�X�N���X��`===
// �v���C���[��ԃx�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Player.h"

// �N���X��`�����邱�Ƃ�錾
class Player;

// ===�N���X��`===
class PlayerStrategy
{
	// ---�����o�֐�---
public:
	virtual void Init(Player* player) = 0;			// ����������
	virtual void Update(Player* player) = 0;		// �X�V����
	virtual bool IsUpdate(Player* player) = 0;		// �X�V���
};
#pragma once
// ===�v���C���[�̍s���p�^�[���N���X
// ---Author kimura
#include "Enemy.h"

// �N���X��`�����邱�Ƃ�錾
class Enemy;

// ===EnemyStrategy�N���X
// ---�v���C���[�̍s���x�[�X
class EnemyState
{
public:
	virtual void Update(Enemy* kakasi) = 0;		// �X�V
	virtual bool IsUpdate(Enemy* kakasi) = 0;		// �J�ڂł��邩�ǂ���
};
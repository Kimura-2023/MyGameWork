#pragma once
// ===�_���[�W�N���X EnemyDamage.h
// --Author kimura

#include "EnemyState.h"

class EnemyMoving : public EnemyState
{
public:
	void Update(Enemy* enemy);
	virtual bool IsUpdate(Enemy* enemy);		// �J�ڂł��邩�ǂ���
};
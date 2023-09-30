#pragma once
// ===ダメージクラス EnemyDamage.h
// --Author kimura

#include "EnemyState.h"

class EnemyMoving : public EnemyState
{
public:
	void Update(Enemy* enemy);
	virtual bool IsUpdate(Enemy* enemy);		// 遷移できるかどうか
};
#pragma once
// ===プレイヤーの行動パターンクラス
// ---Author kimura
#include "Enemy.h"

// クラス定義があることを宣言
class Enemy;

// ===EnemyStrategyクラス
// ---プレイヤーの行動ベース
class EnemyState
{
public:
	virtual void Update(Enemy* kakasi) = 0;		// 更新
	virtual bool IsUpdate(Enemy* kakasi) = 0;		// 遷移できるかどうか
};
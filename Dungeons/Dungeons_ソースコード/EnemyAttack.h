#pragma once
// ===通常敵攻撃クラス定義===
// 通常敵攻撃クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyStrategy.h"

// ---クラス定義
class EnemyAttack : public EnemyStrategy
{
	// ---メンバ関数
public:
	virtual void Init(EnemyBase* enemy);		// 初期化処理
	virtual void Update(EnemyBase* enemy);		// 更新処理

	// ---メンバ変数
private:
	XMFLOAT3 m_vStart;							// 攻撃開始点
};
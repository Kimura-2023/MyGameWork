#pragma once
// ===通常敵クラス定義===
// 通常敵クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "EnemyStrategy.h"
#include "Collision3D.h"
#include "EnemyBase.h"

// ===クラス宣言===
class EnemyNormal : public EnemyBase,Collision3D
{
	// ---メンバ関数
public:
	EnemyNormal();			// コンストラクタ
	~EnemyNormal();			// デストラクタ

	virtual void Init();	// 初期化処理
	virtual void Update();	// 更新処理
	virtual void Draw();	// 描画処理
	void OnCollision(Object*);		// 衝突処理
};
#pragma once
// ===遠距離敵クラス定義===
// 遠距離敵クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "EnemyStrategy.h"
#include "Collision3D.h"
#include "EnemyBase.h"
#include "Bullet.h"

// ===定数・マクロ定義===
#define MAX_BULLET (3)				// 最大弾数

// ===クラス宣言===
class EnemyBullet : public EnemyBase, Collision3D
{
	// ---メンバ関数
public:
	EnemyBullet();				// コンストラクタ
	~EnemyBullet();				// デストラクタ

	void Init();					// 初期化処理
	void Update();					// 更新処理
	void Draw();					// 描画処理
	void OnCollision(Object*);		// 衝突処理
	float& GetView() { return m_view; }			// 視認距離取得
	float& GetInterval() { return m_AttackInterval; }		// 攻撃間隔時間取得
	void FireBullet();			// 弾発射

	// ---メンバ変数
private:
	Bullet* m_pBullets;			// 弾
};
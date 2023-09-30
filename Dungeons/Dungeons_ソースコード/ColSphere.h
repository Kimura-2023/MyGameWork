#pragma once

// ===当たり判定(境界球)クラス定義===
// 当たり判定(境界球)クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "CollisionBase.h"
#include "Sphere.h"

// ===クラス定義===
class CollSphere : public CollisionBase
{
	// ---メンバ関数---
public:
	CollSphere(Collision3D*,float);				// コンストラクタ(半径指定
	CollSphere(Collision3D*, XMFLOAT3, float);	// コンストラクタ(オフセット、半径指定
	~CollSphere();								// デストラクタ

	void Draw();								// デバッグ表示
	bool CheckHit(CollisionBase*);				// 当たり判定
	float GetRadius();							// 境界球半径取得
private:
	void Init();								// 初期化

	// ---メンバ変数---
private:
	float m_Radius;					// 境界球半径
	static CSphere* m_pBSphere;		// デバッグ表示用メッシュ
	static int m_nCnt;				// 境界球総数
};
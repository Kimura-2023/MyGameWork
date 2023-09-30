#pragma once

// ===当たり判定(OBB)クラス定義===
// 当たり判定(OBB)クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "CollisionBase.h"
#include "Box.h"

// ===クラス定義===
class CollOBB : public CollisionBase
{
	// ---メンバ関数---
public:
	CollOBB(Collision3D*, XMFLOAT3);					// コンストラクタ(サイズ指定
	CollOBB(Collision3D*, XMFLOAT3, XMFLOAT3);			// コンストラクタ(オフセット、サイズ指定
	~CollOBB();											// デストラクタ

	void Draw();										// デバッグ表示
	bool CheckHit(CollisionBase*);						// 当たり判定
	XMFLOAT3 GetSize();									// サイズ取得

	void CalcLenOBBtoPoint(float* ans, const XMFLOAT3& point);								// OBBとの最短距離を求める
	static void CalcOBBtoPoint(XMFLOAT3* ans, const XMFLOAT3& point, CollOBB* obb);			// OBBの最近接点を求める
private:
	void Init();										// 初期化

	// ---メンバ変数---
private:
	static CBox* m_pBox;								// デバッグ表示用メッシュ
	static int m_nCnt;									// OBB総数
};

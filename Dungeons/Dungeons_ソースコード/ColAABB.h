#pragma once

// ===当たり判定(AABB)クラス定義===
// 当たり判定(AABB)クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "CollisionBase.h"
#include "Box.h"

// ===クラス定義===
class CollAABB : public CollisionBase
{
	// ---メンバ関数---
public:
	CollAABB(Collision3D*, XMFLOAT3);				// コンストラクタ(サイズ指定
	CollAABB(Collision3D*, XMFLOAT3, XMFLOAT3);		// コンストラクタ(オフセット、サイズ指定
	~CollAABB();									// デストラクタ

	void Draw();									// デバッグ表示
	bool CheckHit(CollisionBase*);					// 当たり判定
	XMFLOAT3 GetSize();								// サイズ取得
private:
	void Init();									// 初期化

	// ---メンバ変数---
private:
	XMFLOAT3 m_Size;			// AABBサイズ
	static CBox* m_pBox;		// デバッグ表示用メッシュ
	static int m_nCnt;			// AABB総数
};
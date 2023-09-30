#pragma once

// ===当たり判定(線分)クラス定義===
// 当たり判定(線分)クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "CollisionBase.h"
#include "Line.h"

// ===クラス定義===
class CollLine : public CollisionBase
{
	// ---メンバ関数---
public:
	CollLine(Collision3D*, XMFLOAT3);				// コンストラクタ(ベクトル指定
	CollLine(Collision3D*, XMFLOAT3, XMFLOAT3);		// コンストラクタ(オフセット、ベクトル指定
	~CollLine();									// デストラクタ

	void Draw();							// デバッグ表示
	bool CheckHit(CollisionBase*);			// ヒット確認

	XMFLOAT3 GetEnd();		// 終点座標取得
	XMFLOAT3 GetVec();		// 始点から終点へのベクトル取得
private:
	void Init();			// 初期化処理

	// ---メンバ変数---
private:
	XMFLOAT3 m_End;			// 終点座標
	XMFLOAT3 m_vec;			// 始点から終点への向き
	static Line* m_Line;	// 線分
	static int m_nCnt;		// 総数
};
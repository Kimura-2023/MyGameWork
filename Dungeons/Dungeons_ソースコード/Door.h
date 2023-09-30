#pragma once
// ===ドアクラス定義===
// ドアクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "Collision3D.h"

// ===クラス定義===
class Door : public Object3DBase, Collision3D
{
	// ---メンバ関数---
public:
	Door();				// コンストラクタ
	~Door();			// デストラクタ

	void Update();		// 更新処理
	void Draw();		// 描画処理

	bool IsUpdate();	// 更新有無
	void OnCollision(Object*);		// 衝突処理

	void SetDirection();								// 向き設定
	int& GetDirection() { return m_nDirection; }		// 向き取得

	XMFLOAT3& GetEntrance() { return m_Entrance; }		// 出現位置取得

	void Open();		// ドア開ける

	// ---メンバ変数---
private:

	int m_nDirection;		// 向き(0,1,2,3)
	XMFLOAT3 m_Entrance;	// 出現位置
};
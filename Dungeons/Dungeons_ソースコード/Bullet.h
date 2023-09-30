#pragma once
// ===弾クラス定義===
// 敵弾クラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Collision3D.h"
#include "Object3DBase.h"

// ===クラス定義===
class Bullet : public Object3DBase, Collision3D
{
	// ---メンバ関数
public:
	Bullet();			// コンストラクタ
	~Bullet();			// デストラクタ
	
	void Update();		// 更新
	void Draw();		// 描画
	bool IsUpdate();	// 更新可否
	void OnCollision(Object*);			// 衝突処理
	void SetRefrect(const XMFLOAT3&);	// 反射状態セット

	bool Fire(const XMFLOAT3& pos, const XMFLOAT3& front, const float& speed);		// 弾発射

	// ---メンバ変数
private:
	bool m_bRefrect;	// 反射されているかフラグ
};
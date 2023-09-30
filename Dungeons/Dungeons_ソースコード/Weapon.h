#pragma once
// ===武器クラス定義===
// 武器のクラスを定義
// Author kimura
// ====================

// ===インクルード===
#include "main.h"
#include "Collision3D.h"
#include "Object3DBase.h"

// ===クラス定義===
class Weapon : public Object3DBase,Collision3D
{
	// ---メンバ関数---
public:
	Weapon(Object3DBase*);				// コンストラクタ(親オブジェクトのポインタ)
	~Weapon();							// デストラクタ

	void Update();						// 更新処理
	bool IsUpdate();					// 更新有無
	void OnCollision(Object*);			// 衝突処理
	void SetResource(std::string);		// リソースセット

	void SetAttack(const bool&);		// 通常攻撃フラグセット
	void SetRepel(const bool&);			// 反射フラグセット

	// ---メンバ変数---
private:
	Object3DBase* m_pParent;			// 親オブジェクト(プレイヤー想定)

	bool m_bAttack;						// 通常攻撃フラグ
	bool m_bRepel;						// 反射フラグ
};
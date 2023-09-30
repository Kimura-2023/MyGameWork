#pragma once
// ===柱クラス定義===
// 柱オブジェクトクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "Collision3D.h"

// ===クラス定義===
class Pillar : public Object3DBase, Collision3D
{
	// ---メンバ関数---
public:
	Pillar();			// コンストラクタ
	~Pillar();			// デストラクタ

	void Update();		// 更新処理
	void Draw();		// 描画処理
	bool IsUpdate();	// 更新有無
	void OnCollision(Object*);		// 衝突処理

	// ---メンバ変数---
private:
};
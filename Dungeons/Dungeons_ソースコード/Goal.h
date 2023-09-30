#pragma once
// ===ゴールオブジェクトクラス定義===
// ゴールオブジェクトクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "SceneManager.h"
#include "Collision3D.h"

// ===クラス定義===
class Goal : public Object3DBase, Collision3D
{
	// ---メンバ関数---
public:
	Goal();					// コンストラクタ
	~Goal();					// デストラクタ

	void Update();				// 更新処理
	void Draw();				// 描画処理
	bool IsUpdate();			// 更新有無
	void OnCollision(Object*);	// 衝突処理

	// ---メンバ変数---
private:
	float m_Rad;				// 上下移動にsinを使うための値
};
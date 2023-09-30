#pragma once

// ===当たり判定管理クラス定義===
// 当たり判定管理クラスを定義
// Author Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "Collision3D.h"
#include <list>

// ===定数・マクロ定義===
#define COLLISION (CollisionManager::Get())			// 当たり判定マネージャのインスタンス取得
using COLIT = std::list<Collision3D*>::iterator;	// 当たり判定リストのイテレーター

// ===クラス定義===
class CollisionManager
{
	// ---メンバ関数---
private:
	CollisionManager();								// コンストラクタ
	~CollisionManager();							// デストラクタ
public:
	static CollisionManager* Get();					// 取得
	static void Destroy();							// 破棄
	static void Create();							// 生成

	void Update();									// 更新処理
	void Draw();									// 描画処理

	void AddManager(Collision3D*);					// 管理下に追加
	void Release();									// 全当たり判定解放
	void Release(Collision3D*);						// 当たり判定解放
	void SwitchVisibleCollision(Object*);			// 可視状態を切替

	// ---メンバ変数---
private:
	static CollisionManager* m_pInstance;			// インスタンス
	std::list<Collision3D*> m_CollisionList;		// 当たり判定全リスト
};

#pragma once

// ===オブジェクト管理クラス定義===
// オブジェクト管理クラスを定義
// Author kimura
// ===================================

// ===インクルード===
#include "main.h"
#include "Object3DBase.h"
#include "Object2DBase.h"
#include <list>
#include "UIBase.h"

// ===定数マクロ定義===
#define OBJECT (ObjectManager::Get())

using O3DIT = std::list<Object3DBase*>::iterator;		// 2Dオブジェクトのイテレーター宣言
using O2DIT = std::list<Object2DBase*>::iterator;		// 3Dオブジェクトのイテレーター宣言

// ===クラス定義===
class ObjectManager
{
	// ---メンバ関数---
private:
	ObjectManager();		// コンストラクタ
	~ObjectManager();		// デストラクタ
public:
	static void Create();			// 生成
	static void Destroy();			// 破棄
	static ObjectManager* Get();	// 取得

	void AddObjectManager(Object3DBase*);	// 3Dオブジェクト追加
	void AddObjectManager(Object2DBase*);	// 2Dオブジェクト追加
	void AddObjectManager(UIBase*);			// UIオブジェクト追加

	void Update();					// 更新
	void Draw();					// 描画
	void Release();					// オブジェクト解放
	void Release(Object3DBase*);	// 指定の3Dオブジェクト解放
	void Release(Object2DBase*);	// 指定の2Dオブジェクト解放
	void Release(UIBase*);			// 指定のUIオブジェクト解放

	// ---メンバ変数---
private:
	static ObjectManager* m_pInstance;		// 自身のインスタンス
	std::list<Object3DBase*> m_3dObject;	// 3Dオブジェクトリスト
	std::list<Object2DBase*> m_2dObject;	// 2Dオブジェクトリスト
	std::list<UIBase*> m_UiObject;			// UIオブジェクトリスト

	int m_nCount;							// 総オブジェクト数
};

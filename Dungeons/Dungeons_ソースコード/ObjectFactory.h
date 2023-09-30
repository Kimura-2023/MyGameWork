#pragma once
// ===オブジェクト生成クラス定義====
// オブジェクト生成を行うクラスを定義
// Author Kimura
// ==============================

// ===インクルード===
#include "Object.h"
#include "main.h"

// ===クラス定義===
class ObjectFactrory
{
	// ---メンバ関数---
public:
	static Object* CreateObject(ObjType);		// オブジェクト生成
};
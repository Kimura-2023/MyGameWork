#pragma once
// ===ゲーム進行管理クラス定義===
// ゲーム管理クラスを定義
// Author Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include "GameCamera.h"
#include "Input.h"
#include "SceneManager.h"

// ===定数・マクロ定義===
#define GAME (GameManager::Get())					// インスタンス取得

// ===クラス定義===
class GameManager
{
	// ---メンバ関数---
public:
	GameManager();									// コンストラクタ
	~GameManager();									// デストラクタ

	static GameManager* Get();						// インスタンス取得
	static void Destroy();							// インスタンス破棄
	static void Create();							// インスタンス生成

	void Update();									// 更新処理
	void Draw();									// 描画処理

	XMFLOAT3 MousePos();							// マウス座標(XZ平面上)取得
	POINT GetMousePoint() { return m_Point; }		// マウススクリーン座標取得

	// ---メンバ変数---
private:
	static GameManager* m_pInstance;				// インスタンス

	POINT m_Point;									// スクリーンマウス座標
	XMFLOAT3 m_MouseWorldPos;						// XZ平面上マウス座標
};

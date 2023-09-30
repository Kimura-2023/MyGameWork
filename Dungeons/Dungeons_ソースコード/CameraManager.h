#pragma once

// ===カメラ管理クラス定義===
// カメラ管理クラスを定義
// Author Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include <string>
#include "Camera.h"
#include <map>

// ===定数・マクロ定義===
#define CAMERA (CameraManager::Get())		// インスタンス取得

// ===カメラ種別定義===
enum CameraType
{
	GAME_CAMERA,			// ゲームシーンカメラ
	DEBUG_CAMERA,			// デバッグシーンカメラ
	TITLE_CAMERA,			// タイトルシーンカメラ
	NONE_CAMERA,
};

// ===クラス定義===
class CameraManager
{
	// ---メンバ関数---
public:
	CameraManager();					// コンストラクタ
	~CameraManager();					// デストラクタ

	static CameraManager* Get();		// インスタンス取得
	static void Destroy();				// インスタンス破棄
	static void Create();				// インスタンス生成

	void Update();						// 更新処理
	void Draw();						// 描画処理

	void RegisterCamera(std::string name,CameraType type);				// カメラ登録
	void SetCamera(CameraBase* camera) { m_CurrentCamera = camera; }	// 現在カメラセット
	CameraBase* GetCurrentCamera() { return m_CurrentCamera; }			// 現在カメラ取得
	CameraBase* FindCamera(std::string);								// カメラ探索

	CameraBase* CreateCamera(CameraType);		// カメラ生成
	void Release();								// カメラ解放

	// ---メンバ変数---
private:
	static CameraManager* m_pInstance;					// インスタンス
	std::map<std::string, CameraBase*> m_CameraList;	// カメラのリスト

	CameraBase* m_CurrentCamera;	// 現在カメラ
};

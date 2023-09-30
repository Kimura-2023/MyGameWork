#pragma once
// ===ゲームカメラクラス定義===
// ゲームカメラクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Camera.h"
#include "Object.h"

// ===クラス定義===
class GameCamera : public CameraBase
{
	// ---メンバ関数---
public:
	GameCamera();								// コンストラクタ
	void Init();								// 初期化処理
	void Update();								// 更新処理
	
	void SetTargetObj(Object*);					// 追従オブジェクト設定
	Object* GetTargetObj() { return m_Target; }	// 追従オブジェクト取得
	void Shake(float second);					// カメラ振動

	// ---メンバ変数---
private:
	Object* m_Target;				// 追従オブジェクト
	XMFLOAT3 m_vDestPos;			// 目標座標
	XMFLOAT3 m_vDestTarget;			// 目標注視点
	float m_fShakeFrame;			// 振動時間
	float m_fShakeRad;				// 振動位置(三角関数で-1～1の値に納める)
};
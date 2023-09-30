// ===ゲームカメラクラス実装===
// ゲームカメラクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "GameCamera.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "Calc.h"

// ===グローバル変数===
namespace {
	const float VALUE_MOVE_CAMERA = 0.5f;			// カメラ移動量
	const float VALUE_ROTATE_CAMERA = 0.5f;			// カメラ回転量

	const float POS_P_X = 8.0f;						// 三人称カメラの視点位置(X座標)
	const float POS_P_Y = 18.0f;					// カメラの視点初期位置(Y座標)
	const float POS_P_Z = -12.0f;					// カメラの視点初期位置(Z座標)
	const float POS_R_X = 0.0f;						// 三人称カメラの注視点位置(X座標)
	const float POS_R_Y = 0.0f;						// 三人称カメラの注視点位置(Y座標)
	const float POS_R_Z = 0.0f;						// 三人称カメラの注視点位置(Z座標)
}

// ===コンストラクタ===
GameCamera::GameCamera()
{
	// ---初期化処理---
	Init();			// 初期化
	m_fShakeFrame = 0.0f;		// 振動時間を0に設定
	m_fShakeRad = 0.0f;			// 振動位置を0に設定
}

// ===初期化処理===
void GameCamera::Init()
{
	CameraBase::Init();					// カメラ初期化

	m_vDestPos = m_vPos;				// 視点セット
	m_vDestTarget = m_vTarget;			// 注視点セット

	m_Target = nullptr;					// 追従するオブジェクトをセット
}

// ===更新===
void GameCamera::Update()
{
	// ---追従するか判定---
	if (m_Target != nullptr) {

		XMFLOAT3 p = m_Target->GetTransform().Pos;

		// ---目的座標---
		m_vDestPos.x = POS_P_X + p.x;			// ---視点座標移動先を算出
		m_vDestPos.y = POS_P_Y + p.y;
		m_vDestPos.z = POS_P_Z + p.z;
		m_vDestTarget.x = POS_R_X + p.x;		// ---注視点座標移動先を算出
		m_vDestTarget.y = POS_R_Y + p.y;
		m_vDestTarget.z = POS_R_Z + p.z;

		// ---座標更新---
		m_vPos.x = m_vPos.x * 0.92f + m_vDestPos.x * 0.08f;				// ---視点を徐々に目的座標に近づける
		m_vPos.y = m_vPos.y * 0.92f + m_vDestPos.y * 0.08f;
		m_vPos.z = m_vPos.z * 0.92f + m_vDestPos.z * 0.08f;

		m_vTarget.x = m_vTarget.x * 0.92f + m_vDestTarget.x * 0.08f;	// ---注視点座標を徐々に移動先に近づける
		m_vTarget.y = m_vTarget.y * 0.92f + m_vDestTarget.y * 0.08f;
		m_vTarget.z = m_vTarget.z * 0.92f + m_vDestTarget.z * 0.08f;
	}

	// ---追従しない場合---
	else
	{
		m_vPos.x = POS_P_X;			// ---初期座標をセット
		m_vPos.y = POS_P_Y;
		m_vPos.z = POS_P_Z;
	}

	// ---カメラ振動処理---
	if (m_fShakeFrame > 0.0f)
	{
		--m_fShakeFrame;											// 振動時間を更新
		m_fShakeRad += XMConvertToRadians(m_fShakeFrame * 6.0f);		// 揺れの大きさを一定数値に納める

		if (m_fShakeRad > XM_PI * 2.0f)
		{
			m_fShakeRad -= XM_PI * 2.0f;									// 
		}

		m_vPosGap.x = (m_fShakeFrame / 60.0f) * cosf(m_fShakeRad);								// 振動後の位置を算出

		if (m_fShakeFrame == 0.0f)
		{
			m_fShakeFrame = 0.0f;									// 振動時間0でリセット
			m_vPosGap = XMFLOAT3(0.0f, 0.0f, 0.0f);					// 座標ずれをリセット
		}
	}

	CameraBase::Update();			// カメラ更新
}

// ===追従オブジェクト設定===
void GameCamera::SetTargetObj(Object* obj)
{
	m_Target = obj;				// ターゲット設定
	if (!m_Target)
		return;
	m_vTarget = m_Target->GetTransform().Pos;		// 注視点を設定
}

// ===カメラ振動処理===
void GameCamera::Shake(float second)
{
	m_fShakeFrame = second * 60.0f;					// 振動時間を設定
}
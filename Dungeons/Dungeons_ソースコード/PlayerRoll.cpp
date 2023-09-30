// ===プレイヤー回避クラス実装===
// プレイヤー回避クラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "PlayerRoll.h"
#include "Input.h"
#include "Sound.h"
#include "GameManager.h"
#include "DefineData.h"
#include "EffectManager.h"
#include "CameraManager.h"

// ===初期化処理===
void PlayerRoll::Init(Player* player)
{
	if (player->GetStamina() <= 10)			// スタミナで判定
		return;

	// ---パラメータ初期化---
	player->SetAnimeTime(0.0f);				// アニメーション時間を設定
	player->SetAnimeRate(0.0f);				// アニメーション遷移率を設定
	player->SetAnimeLoop(false);			// ループ設定
	player->DelStamina(10);					// スタミナ減少

	CSound::Play(SE_DUSH);					// ダッシュ音再生

	// ---向き計算---

	XMFLOAT3 vec = player->GetFront();
	if (CInput::GetKeyTrigger(VK_SPACE))			// --マウスの場合
	{
		vec = GAME->MousePos();						// マウス座標を取得
		XMFLOAT3 pos = player->GetTransform().Pos;	// プレイヤーの座標を取得
		vec.x -= pos.x;				// 向き(x方向)を算出
		vec.z -= pos.z;				// 向き(z方向)を算出
		vec.y = 0.0f;				// 高さは無視する
		Normalize(&vec);			// ベクトルを正規化
		player->SetFront(vec);		// プレイヤーの正面方向を算出したベクトルに設定
	}
	else											// ----コントローラー
	{
		XMFLOAT2 stick = XMFLOAT2(0.0f, 0.0f);		// スティック入力方向
		stick.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));			// スティックX方向の入力値を取得
		stick.y = -CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));			// スティックY方向の入力値を取得

		// ---スティック値の補正---
		if (fabs(stick.x) >= DEAD_STICK_X)			// デッドゾーンを超えていれば値を反映
		{
			Normalize(&stick);		// 入力方向を正規化(スティックの倒し具合で速度を変えたい場合は修正)
			vec.x = stick.x;		// 進行方向にスティックの値(X)を反映
		}
		if (fabs(stick.y) >= DEAD_STICK_Y)			// デッドゾーンを超えていれば値を反映
		{
			Normalize(&stick);		// 入力方向を正規化(スティックの倒し具合で速度を変えたい場合は修正)
			vec.z = stick.y;		// 進行方向にスティックの値(Y)を反映
		}
		Normalize(&vec);

		// ---向きの計算---
		XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();	// カメラの視点ベクトルを取得
		Quaternion q;													// 回転クォータニオン
		view.y = 0;														// プレイヤーはY軸回転させたいので視点ベクトルのYの値は無視する
		Quaternion::LookRotation(&q, view);								// 視点方向を向くクォータニオンを算出

		Quaternion p(vec);												// 入力方向
		p = -q * p * q;													// 回転クォータニオンで挟み込み、進行方向の補正をする
		vec = XMFLOAT3(p.x, p.y, p.z);									// 求めた向きを新しい進行方向とする
		XMFLOAT3 input = vec;
		vec.y = player->GetVel().y;												// 進行速度(Y)を取得
		float speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);		// 速度を求める
		if (speed != 0)				// 進行速度が正しく求められている場合
			player->SetFront(input);				// プレイヤーの正面向きを進行方向にセット
	}

	// ---速度設定---
	player->SetSpeed(PLAYER_SPEED * 6.0f);			// 速度設定
	vec.x *= player->GetSpeed();					// 進行速度を算出(x方向)
	vec.z *= player->GetSpeed();					// 進行速度を算出(y方向)
	player->SetVel(vec);							// 速度を設定
	player->SetAcc(XMFLOAT3(player->GetAcc().x,0.0f,player->GetAcc().z));		// 加速度設定

	// ---アニメーション設定---
	player->PlayerSetBlendAnime(ANIM_ROLL);			// アニメーション遷移設定
}

// ===更新処理===
void PlayerRoll::Update(Player* player)
{
	player->SetGravity(false);						// 重力無効化
	if (player->GetEndAnime())						// アニメーション終了判定
		player->SetStrategy(STAY_PLAYER);			// プレイヤー状態遷移
	EFFECT->Play(MOVE_EFFECT, player->GetPos());	// エフェクト再生
}

// ===更新有無===
bool PlayerRoll::IsUpdate(Player* player)
{
	return player->GetEndAnime();					// アニメーション終了判定
}
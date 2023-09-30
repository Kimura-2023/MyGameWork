// ===プレイヤー移動クラス実装===
// プレイヤー移動クラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "PlayerMove.h"
#include "Input.h"
#include "DefineData.h"
#include "CameraManager.h"

// ===初期化処理===
void PlayerMove::Init(Player* player)
{
	player->SetAnimeRate(0.1f);					// アニメーション遷移率初期化
	player->PlayerSetBlendAnime(ANIM_RUN);		// アニメーションを走りにセット(ブレンド再生)
	player->SetAnimeLoop(true);					// アニメーションループ

	// ---コントローラー入力---
	if (CInput::GetJoyCount() > 0)				// コントローラー接続数が1台以上あれば
		m_bController = true;
	else
		m_bController = false;

}

// ===更新処理===
void PlayerMove::Update(Player* player)
{
	// ---変数宣言---
	XMFLOAT3 vel = { 0,0,0 };					// 進行方向
	XMFLOAT2 stick = XMFLOAT2(0.0f,0.0f);		// スティック入力方向

	// ---コントローラー入力---
	if (m_bController)				// コントローラー接続数が1台以上あれば
	{
		stick.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));			// スティックX方向の入力値を取得
		stick.y = -CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));			// スティックY方向の入力値を取得
	}

	// ---キー入力---
	if (CInput::GetKeyPress(VK_A)) vel.x = -1.0f;							// 左方向
	if (CInput::GetKeyPress(VK_W)) vel.z =  1.0f;							// 奥方向
	if (CInput::GetKeyPress(VK_S)) vel.z = -1.0f;							// 後方向
	if (CInput::GetKeyPress(VK_D)) vel.x =  1.0f;							// 右方向

	// ---スティック値の補正---
	if (fabs(stick.x) >= DEAD_STICK_X)			// デッドゾーンを超えていれば値を反映
	{
		Normalize(&stick);		// 入力方向を正規化(スティックの倒し具合で速度を変えたい場合は修正)
		vel.x = stick.x;		// 進行方向にスティックの値(X)を反映
	}
	if (fabs(stick.y) >= DEAD_STICK_Y)			// デッドゾーンを超えていれば値を反映
	{
		Normalize(&stick);		// 入力方向を正規化(スティックの倒し具合で速度を変えたい場合は修正)
		vel.z = stick.y;		// 進行方向にスティックの値(Y)を反映
	}

	// ---例外処理---
	if (vel.x == 0.0f && vel.z == 0.0f)
	{
		player->SetVel(vel);
		player->SetStrategy(STAY_PLAYER);		// 待機状態に遷移
		return;
	}
	Normalize(&vel);							// 進行方向ベクトルを正規化

	// ---向きの計算---
	XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();	// カメラの視点ベクトルを取得
	Quaternion q;													// 回転クォータニオン
	view.y = 0;														// プレイヤーはY軸回転させたいので視点ベクトルのYの値は無視する
	Quaternion::LookRotation(&q,view);								// 視点方向を向くクォータニオンを算出

	Quaternion p(vel);												// 入力方向
	p = -q * p * q;													// 回転クォータニオンで挟み込み、進行方向の補正をする
	vel = XMFLOAT3(p.x, p.y, p.z);									// 求めた向きを新しい進行方向とする
	vel.x *= PLAYER_SPEED * 3.0f;									// 進行速度(X)を求める
	vel.z *= PLAYER_SPEED * 3.0f;									// 進行速度(Z)を求める

	XMFLOAT3 input = vel;											// Y方向の速度を無視した値を格納

	vel.y = player->GetVel().y;										// 進行速度(Y)を取得
	player->SetVel(vel);											// 進行速度をプレイヤーにセット

	float speed = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);		// 速度を求める
	if (speed != 0)				// 進行速度が正しく求められている場合
		player->SetFront(input);				// プレイヤーの正面向きを進行方向にセット

	player->DelStamina(-0.5);					// スタミナを回復
}

// ===更新状態取得===
bool PlayerMove::IsUpdate(Player* player)
{
	return 	true;
}
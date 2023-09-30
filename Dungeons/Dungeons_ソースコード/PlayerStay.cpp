// ===プレイヤー待機クラス実装===
// プレイヤー待機クラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "PlayerStay.h"

// ===初期化===
void PlayerStay::Init(Player* player)
{
	// ---パラメータ初期化---
	player->SetAnimeLoop(true);					// アニメーションループ設定
	player->SetAnimeRate(0.1f);					// アニメーション遷移率設定
	player->SetAtk(false);						// 攻撃状態設定
	player->PlayerSetBlendAnime(ANIM_IDLE);		// アニメーション遷移
}

// ===更新処理===
void PlayerStay::Update(Player* player)
{
	// ---速度設定---
	float speed = player->GetSpeed();			// 速度取得
	XMFLOAT3 vel = player->GetVel();

	speed--;									// 減速
	if (speed <= 0.0f)
		speed = 0.0f;							// 最小値に納める
	vel.x *= speed;
	vel.z *= speed;
	player->SetVel(vel);						// 進行速度設定

	// ---パラメータ更新---
	if (player->GetRate() >= 1.0f)				// アニメーション遷移率で判定
	{
		player->SetAnimeLoop(true);				// アニメーションループ設定
	}

	player->DelStamina(-1);						// スタミナ更新
}

// ===更新有無===
bool PlayerStay::IsUpdate(Player* player)
{
	return true;
}
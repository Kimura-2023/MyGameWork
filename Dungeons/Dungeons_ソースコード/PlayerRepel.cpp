// ===プレイヤー弾きクラス実装===
// プレイヤー弾きクラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "PlayerRepel.h"
#include "Input.h"
#include "GameManager.h"
#include "DefineData.h"

// ===プレイヤー攻撃初期化===
void PlayerRepel::Init(Player* player)
{
	// ---アニメーション初期化---
	player->SetAnimeTime(0.0f);
	player->SetAnimeRate(0.0f);
	player->PlayerSetBlendAnime(ANIM_REPEL);
	player->SetAnimeLoop(false);

	// ---向き計算---
	XMFLOAT3 vec;
	if (CInput::GetMouseTrigger(MOUSEBUTTON_R))			// --マウスの場合
	{
		vec = GAME->MousePos();							// マウス座標取得
		XMFLOAT3 pos = player->GetTransform().Pos;		// プレイヤー座標取得
		vec.x -= pos.x;									// マウスへのベクトルを算出
		vec.z -= pos.z;									// マウスへのベクトルを算出
		vec.y = 0.0f;									// 高さは計算しない
	}
	else												// ----コントローラー
	{
		vec = player->GetFront();						// 向いている方向に弾く
	}
	Normalize(&vec);								// 求めたベクトルを正規化
	player->SetFront(vec);							// プレイヤーの向きを計算したベクトルにする

	// ---速度設定---
	player->SetSpeed(PLAYER_SPEED * 0.1f);				// 速度セット
	vec.x *= player->GetSpeed();
	vec.z *= player->GetSpeed();
	player->SetVel(vec);

	// ---武器に弾き状態をセット---
	Weapon* weapon = player->GetWeapon();
	weapon->SetRepel(true);
}

// ===プレイヤー弾き更新===
void PlayerRepel::Update(Player* player)
{
	// ---取得---
	int nowAnime = player->GetAnimeNo();		// 現在アニメーション番号取得
	float nowTime = player->GetAnimTime();		// 現在アニメーション時間取得
	Model* model = player->GetModel();			// プレイヤーモデル取得
	Weapon* weapon = player->GetWeapon();		// 武器取得


	// ---待機モーションに切替---
	if (player->GetEndAnime())
	{
		player->SetStrategy(STAY_PLAYER);		// 待機状態に切替
		//weapon->SetRepel(false);				// 弾き無効化
		weapon->SetCol(false);					// 武器判定無効化
		return;
	}

	// ---弾き判定---
	if (nowTime * model->GetAnimTick(nowAnime) / 60.0f <= model->GetAnimDuration(nowAnime) * 0.7f)			// 一定フレーム以下は弾き有効
		weapon->SetCol(true);					// 武器判定有効化
	else
		weapon->SetCol(false);					// 武器判定無効化
}

// ===更新有無===
bool PlayerRepel::IsUpdate(Player* player)
{
	return player->GetEndAnime();				// アニメーション終了判定
}
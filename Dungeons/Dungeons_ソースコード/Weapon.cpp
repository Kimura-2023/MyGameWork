// ===武器クラス実装===
// 武器クラスを実装
// Author:	Kimura
//======================

// ===インクルード===
#include "Weapon.h"
#include "CollisionManager.h"
#include "DebugProc.h"
#include "TimeController.h"
#include "Sound.h"
#include "EffectManager.h"
#include "EnemyBase.h"
#include "Bullet.h"
#include "Player.h"

// ===コンストラクタ===
Weapon::Weapon(Object3DBase* pobj) : Collision3D(this), m_pParent(pobj)			// プレイヤーをセット
{
	// ---初期化---
	m_id = PLAYER_ATACK;								// タグ
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期座標
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 回転
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);		// 拡縮
	m_fRadius = 0.5f;									// 半径
	m_bEnable = true;									// 更新・描画可否
	m_col = false;										// 当たり判定可否
	m_bGravity = false;									// 重力

	AddCollisionOBB(XMFLOAT3(0.0f, 0.0f, -0.8f), XMFLOAT3(0.5f, 0.3f, 0.3f));	// 当たり判定追加(OBB)
	SetCollider(true);									// めり込み戻しの計算なし

	m_bAttack = false;		// 攻撃フラグ
	m_bRepel = false;		// 反射フラグ

	//COLLISION->SwitchVisibleCollision(this);
}

// ===デストラクタ===
Weapon::~Weapon()
{
}

// ===更新有無===
bool Weapon::IsUpdate()
{
	return true;
}

// ===更新処理===
void Weapon::Update()
{
	m_transform = m_pParent->GetTransform();							// 親の情報を取得
	m_vFront = m_pParent->GetFront();									// 親の向きを取得
	Object3DBase::Update();												// 更新処理

	// ---アニメーションに沿って動かす---
	XMMATRIX Bone = m_pParent->GetModel()->GetBoneMatrix("hand_R");		// ボーン(hand_R)を取得
	Bone = XMMatrixTranspose(Bone);										// 計算用計算
	XMFLOAT4X4 sword;													// 格納用
	Bone = Bone * XMLoadFloat4x4(&Object3DBase::GetWorld());			// 親の行列と乗算し、姿勢を求める
	XMStoreFloat4x4(&sword, Bone);										// 変換
	Object3DBase::SetWorld(sword);										// 求めた行列をワールド変換行列とする
}

// ===衝突時===
void Weapon::OnCollision(Object* obj)
{
	// ---通常攻撃時---
	if ((obj->GetObjID() == OT_ENEMY || obj->GetObjID() == OT_ENEMY_BULLET) && m_bAttack)
	{
		if (!obj->GetInvincible())							// 無敵判定
		{
			obj->SetInvincible(20);							// 無敵時間をセット
			TIME->SetStop(0.1f);							// ヒットストップ発生(0.1秒)
			CSound::Play(SE_SWORD);							// 効果音再生
			EFFECT->Play(HIT_EFFECT, GetHitPos());			// エフェクト再生(ヒットエフェクト)
			EFFECT->Play(EXPLOSION_EFFECT, GetHitPos());	// エフェクト再生(爆発)
			EFFECT->Play(SWORD_EFFECT, GetHitPos());		// エフェクト再生(剣)
			((EnemyBase*)obj)->DelLife();					// 敵ライフ減少
		}
	}

	// ---敵弾反射時---
	if (obj->GetObjID() == ENEMY_ATTACK && m_bRepel)
	{
		if (!obj->GetInvincible())										// 無敵判定
		{	
			TIME->SetStop(0.3f);										// ヒットストップ発生(0.3秒)
			m_pParent->SetInvincible(10);								// 反射している最中はプレイヤーに無敵時間を発生
			((Player*)m_pParent)->PlayerSetBlendAnime(ANIM_ATTACK2);	// アニメーション遷移(剣を振らせる)
			((Player*)m_pParent)->SetAnimeLoop(false);					// アニメーションループ設定
			((Player*)m_pParent)->SetAnimeRate(0.0f);					// アニメーション遷移率をセット
			((Player*)m_pParent)->SetAnimeTime(0.0f);					// アニメーション時間をセット
			CSound::Play(SE_REPEL);										// 効果音再生
			EFFECT->Play(HIT_EFFECT, GetHitPos());						// エフェクト再生(ヒット)
			EFFECT->Play(REPEL_EFFECT, GetHitPos());					// エフェクト再生(反射)
			((Bullet*)obj)->SetRefrect(m_pParent->GetFront());			// 弾を向いている方向に跳ね返す
		}
	}
}

// ===リソースセット===
void Weapon::SetResource(std::string)
{
}

// ===反射フラグセット===
void Weapon::SetRepel(const bool& flg)
{
	m_bRepel = flg;
	m_bAttack = !flg;
}

// ===通常攻撃フラグセット===
void Weapon::SetAttack(const bool& flg)
{
	m_bAttack = flg;
	m_bRepel = !flg;
}
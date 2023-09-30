// ===弾クラス実装===
// 敵の弾クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Bullet.h"
#include "CollisionManager.h"
#include "DebugProc.h"
#include "TimeController.h"
#include "Sound.h"
#include "EffectManager.h"
#include "EnemyBase.h"
#include "ModelManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "Player.h"

// ===コンストラクタ===
Bullet::Bullet() : Collision3D(this)
{
	m_id = ENEMY_ATTACK;
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_fRadius = 0.5f;
	m_bEnable = false;
	m_col = false;
	m_bGravity = false;
	AddCollisionOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.8f));
	SetCollider(true);															// 当たり判定は取るが、戻しの計算はしない

	m_ResourcePass = "data/model/Bullet.fbx";
	m_Model = MODEL->Load("data/model/Bullet.fbx");

	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);

	m_bRefrect = false;
}

// ===デストラクタ===
Bullet::~Bullet()
{
}

// ===更新可否取得===
bool Bullet::IsUpdate()
{
	return true;
}

// ===更新処理===
void Bullet::Update()
{
	Object3DBase::Update();				// オブジェクト更新
}

// ===描画処理===
void Bullet::Draw()
{
	// ---通常描画---
	SetCullMode(CULLMODE_CCW);
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");		// 諧調テクスチャ取得
	SetTexturePS(pTex, 1);														// 諧調テクスチャセット
	Object3DBase::Draw();			// 描画処理

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);		// 前面カリング設定
	m_pPS = GetPS(PS_OUTLINE);		// 輪郭線ピクセルシェーダーをセット
	m_pVS = GetVS(VS_OUTLINE);		// 輪郭線頂点シェーダーをセット
	Object3DBase::Draw();			// 描画処理

	// ---シェーダーを元に戻す---
	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);

	SetCullMode(CULLMODE_NONE);		// カリング設定リセット
}

// ===衝突時処理===
void Bullet::OnCollision(Object* obj)
{
	// ---プレイヤー衝突時---
	if (obj->GetObjID() == OT_PLAYER && m_bRefrect == false)
	{
		if (!obj->GetInvincible())			// 無敵状態を判断
		{
			obj->SetInvincible(30);		// 無敵時間をセット
			((Player*)obj)->DelLife();	// ライフ減少
			TIME->SetStop(0.1f);		// ヒットストップ発生
		}
	}

	// ---敵衝突時---
	if (m_bRefrect && (obj->GetObjID() == OT_ENEMY || obj->GetObjID() == OT_ENEMY_BULLET))		// 弾が反射されていたら
	{
		obj->SetInvincible(20);			// 無敵時間をセット
		TIME->SetStop(0.2f);			// ヒットストップ発生
		CSound::Play(SE_SWORD);			// 効果音再生
		EFFECT->Play(HIT_EFFECT, GetHitPos());			// 衝突エフェクト発生
		EFFECT->Play(EXPLOSION_EFFECT, GetHitPos());	// 爆発エフェクト発生
		((EnemyBase*)obj)->DelLife(3);	// 敵ライフ減少

		// ---弾初期化
		m_bEnable = false;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_col = false;
		m_bRefrect = false;
	}

	// ---武器、敵衝突時---
	if (obj->GetObjID() != PLAYER_ATACK && obj->GetObjID() != OT_ENEMY && obj->GetObjID() != OT_ENEMY_BULLET)
	{
		// ---弾初期化
		m_bEnable = false;
		m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_col = false;
		m_bRefrect = false;
	}
}

// ===弾発射処理===
bool Bullet::Fire(const XMFLOAT3& pos, const XMFLOAT3& front, const float& speed)
{
	if (m_bEnable)			// 更新状態で判定
		return false;

	CSound::Play(SE_SHOT);	// 効果音再生
	m_transform.Pos = pos;	// 座標をセット
	m_vFront = front;		// 正面方向をセット

	Normalize(&m_vFront);	// 正面方向を正規化

	m_vVel.x = m_vFront.x * speed;		// 向いている方向(x方向)に速度をセット
	m_vVel.y = m_vFront.y * speed;		// 向いている方向(y方向)に速度をセット
	m_vVel.z = m_vFront.z * speed;		// 向いている方向(z方向)に速度をセット

	m_bEnable = true;		// 更新、描画をする状態にする
	m_col = true;			// 当たり判定をとる

	return true;
}

// ===弾反射処理===
void Bullet::SetRefrect(const XMFLOAT3& front)
{
	m_bRefrect = true;					// 反射状態をセット

	m_vFront = front;					// 引数の方向に正面方向をセット
	m_vFront.y = 0.0f;					// 

	Normalize(&m_vFront);				// 正規化

	m_vVel.x = m_vFront.x * 0.5f;		// 向いている方向(x方向)に速度をセット
	m_vVel.y = m_vFront.y * 0.5f;		// 向いている方向(y方向)に速度をセット
	m_vVel.z = m_vFront.z * 0.5f;		// 向いている方向(z方向)に速度をセット
	
	m_bEnable = true;;		// 更新、描画をする状態にする
	m_col = true;	 		// 当たり判定をとる
}
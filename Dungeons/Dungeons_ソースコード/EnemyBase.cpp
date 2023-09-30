// ===敵ベースクラス実装===
// 敵ベースクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "EnemyBase.h"
#include "EnemyMove.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "Player.h"
#include "Sound.h"
#include "SceneManager.h"
#include "EnemyAttack.h"
#include "EffectManager.h"

// ===コンストラクタ===
EnemyBase::EnemyBase()
{
	// ---初期化処理---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 初期座標
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);			// 初期回転
	m_transform.Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);			// 初期拡縮
	m_fRadius = 2.5f;										// 境界球サイズ
	m_bEnable = true;										// 更新・描画可否
	m_col = true;											// 当たり判定可否
	m_bGravity = true;										// 重力可否
	m_view = 5.0f;											// プレイヤー視認距離
	m_ResourcePass = ("data/model/Enemy1.fbx");				// モデル素材へのパス
	m_Model = MODEL->Load("data/model/Enemy1.fbx");			// モデル読込
	m_nLife = 3;											// 体力
	m_pPS = GetPS(PS_TOON);									// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);								// 頂点シェーダーセット
	m_State = STAY_ENEMY;									// 待機状態に設定
	bNearFlg = false;										// 近距離判定セット
}

// ===デストラクタ===
EnemyBase::~EnemyBase()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		delete m_pStrategy[i];
	}
}

// ===更新処理===
void EnemyBase::Update()
{
}

// ===描画処理===
void EnemyBase::Draw()
{
	// ---通常描画---
	SetCullMode(CULLMODE_CCW);

	// 諧調テクスチャセット
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");
	SetTexturePS(pTex, 1);

	Object3DBase::Draw();

	// ---輪郭線描画----
	SetCullMode(CULLMODE_CW);
	m_pPS = GetPS(PS_OUTLINE);
	m_pVS = GetVS(VS_OUTLINE);
	XMFLOAT4 color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	if (bNearFlg)
	{
		SCENE->GetCurrentScene()->SetColorBuffer(&color);
	}
	Object3DBase::Draw();
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	SCENE->GetCurrentScene()->SetColorBuffer(&color);

	// ---描画状態リセット---
	m_pPS = GetPS(PS_TOON);
	m_pVS = GetVS(VS_OBJECT);
	SetCullMode(CULLMODE_NONE);
}

// ===更新可否取得===
bool EnemyBase::IsUpdate()
{
	return true;
}

// ===状態遷移===
void EnemyBase::SetStrategy(ENEMY_STATE enemy)
{
	m_State = enemy;					// 状態をセット
	m_pStrategy[m_State]->Init(this);	// 初期化処理を呼ぶ
}
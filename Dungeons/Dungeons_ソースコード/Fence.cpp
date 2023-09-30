// ===柵クラス実装===
// 柵クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Fence.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"

// ===コンストラクタ===
Fence::Fence() : Collision3D(this)
{
	// ---初期化処理---
	m_transform.Pos = { 0,0,0 };				// 初期座標
	m_transform.Rot = { 0,0,0 };				// 初期回転
	m_transform.Scale = { 1,1,1 };				// 初期拡縮
	m_id = OT_FENCE;							// オブジェクトの型設定
	m_bEnable = true;							// 更新・描画可否
	m_col = true;								// 当たり判定有無
	m_bVisible = true;							// 視認可否
	m_ResourcePass = "data/model/Fence.fbx";	// 素材へのパス
	m_Model = MODEL->Load(m_ResourcePass);		// モデル読込
	AddCollisionOBB(XMFLOAT3(0, 1.0f, 0), XMFLOAT3(1.5f, 2.0f, 0.2f));		// 当たり判定追加
	m_bStatic = true;							// 静的オブジェクト

	m_pPS = GetPS(PS_TOON);						// ピクセルシェーダー設定
	m_pVS = GetVS(VS_OBJECT);					// 頂点シェーダ設定
}

// ===デストラクタ===
Fence::~Fence()
{
}

// ===更新処理===
void Fence::Update()
{
	Object3DBase::Update();			// オブジェクト更新
}

// ===描画処理===
void Fence::Draw()
{
	// ---通常描画---
	SetCullMode(CULLMODE_CCW);		// 背面カリング
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);		// 諧調テクスチャセット
	Object3DBase::Draw();			// オブジェクト描画

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);		// 前面カリング
	m_pPS = GetPS(PS_OUTLINE);		// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OUTLINE);		// 頂点シェーダーセット
	Object3DBase::Draw();			// 描画処理

	// ---描画設定を元に戻す---
	SetCullMode(CULLMODE_CCW);		// 背面カリング
	m_pPS = GetPS(PS_TOON);			// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);		// 頂点シェーダーセット

}

// ===更新有無===
bool Fence::IsUpdate()
{
	return true;
}

// ===衝突処理===
void Fence::OnCollision(Object* obj)
{
}
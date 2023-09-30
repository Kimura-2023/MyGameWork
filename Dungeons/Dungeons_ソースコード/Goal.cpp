// ===ゴールオブジェクトクラス実装===
// ゴールオブジェクトクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Goal.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"

// ===コンストラクタ===
Goal::Goal() : Collision3D(this)
{
	// ---初期化処理---
	m_transform.Pos = { 0,0,0 };				// 初期座標
	m_transform.Rot = { 0,0,0 };				// 初期回転
	m_transform.Scale = { 1,1,1 };				// 初期拡縮
	m_id = OT_GOAL;								// オブジェクトの型設定
	m_bEnable = true;							// 更新・描画可否
	m_col = true;								// 当たり判定有無
	m_bVisible = true;							// 視認可否
	m_ResourcePass = "data/model/goal.fbx";		// 素材へのパス
	m_Model = MODEL->Load(m_ResourcePass);		// モデル読込
	AddCollisionSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);		// 当たり判定追加
	m_bStatic = true;							// 静的オブジェクト

	m_pPS = GetPS(PS_PHONG);					// ピクセルシェーダー設定
	m_pVS = GetVS(VS_OBJECT);					// 頂点シェーダ設定
	m_Rad = 0.0f;
}

// ===デストラクタ===
Goal::~Goal()
{
}

// ===更新処理===
void Goal::Update()
{
	// ---徐々に上下移動---
	++m_Rad;				// sinfで-1～1の値を取得するために値を更新
	if (m_Rad > 360.0f)				// 一定以上(360を超えたらリセット
		m_Rad = 0;					// 値をリセット
	m_transform.Pos.y += sinf(m_Rad * M_PI / 180.0f) * 0.005f;			// 座標に反映させる

	Object3DBase::Update();			// オブジェクト更新
}

// ===描画処理===
void Goal::Draw()
{
	// ---通常描画---
	SetCullMode(CULLMODE_CCW);								// 背面カリング
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// 諧調テクスチャセット
	Object3DBase::Draw();									// オブジェクト描画

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);								// 前面カリング
	m_pPS = GetPS(PS_OUTLINE);								// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OUTLINE);								// 頂点シェーダーセット
	XMFLOAT4 color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);		// 紫色(怪しい感じがする
	SCENE->GetCurrentScene()->SetColorBuffer(&color);		// 色を定数バッファにセット
	Object3DBase::Draw();									// 描画処理

	// ---描画設定を元に戻す---
	SetCullMode(CULLMODE_CCW);		// 背面カリング
	m_pPS = GetPS(PS_PHONG);		// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);		// 頂点シェーダーセット
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);				// 元の色に戻す
	SCENE->GetCurrentScene()->SetColorBuffer(&color);		// 定数バッファにセット

}

// ===更新有無===
bool Goal::IsUpdate()
{
	return true;
}

// ===衝突処理===
void Goal::OnCollision(Object* obj)
{
}
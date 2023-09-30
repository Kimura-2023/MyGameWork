// ===壁柱クラス実装===
// 壁柱オブジェクトクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "WallPillar.h"
#include "ModelManager.h"
#include "ShaderList.h"
#include "CollisionManager.h"
#include "TextureManager.h"
#include "ImGuiBase.h"

// ===コンストラクタ===
WallPillar::WallPillar() : Collision3D(this)
{
	m_transform.Pos = { 0,0,0 };										// 初期座標
	m_transform.Rot = { 0,0,0 };										// 初期回転
	m_transform.Scale = { 1,1,1 };										// 初期拡縮
	m_id = OT_WALLPILLAR;												// オブジェクトの型設定
	m_bEnable = true;													// 更新・描画可否
	m_col = true;														// 当たり判定有無
	m_bVisible = true;													// 視認可否
	m_Model = MODEL->Load("data/model/WallPillar.fbx");					// モデル読込
	AddCollisionOBB(XMFLOAT3(0, 2.5f, 0), XMFLOAT3(0.7f, 2.5f, 0.7f));	// 当たり判定追加
	m_bStatic = true;													// 静的オブジェクト

	m_pPS = GetPS(PS_MASKOBJECT);										// ピクセルシェーダー設定
	m_pVS = GetVS(VS_OBJECT);											// 頂点シェーダー設定

	m_ResourcePass = "data/texture/BlockTex004.png";					// テクスチャパス
	m_pTex = TEX->Load("data/texture/BlockTex004.png");					// テクスチャ読込
}

// ===デストラクタ===
WallPillar::~WallPillar()
{

}

// ===更新処理===
void WallPillar::Update()
{
	Object3DBase::Update();			// オブジェクト更新
}

void WallPillar::Draw()
{
	// ---通常描画---
	SetCullMode(CULLMODE_CCW);								// 背面カリング
	SetTexturePS(m_pTex, 0);								// テクスチャセット
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// 諧調テクスチャセット
	Object3DBase::Draw();									// 描画処理

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);			// 前面カリング
	m_pPS = GetPS(PS_BEHIND);			// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OUTLINE);			// 頂点シェーダーセット
	Object3DBase::Draw();				// 描画処理

	// --- 描画設定を元に戻す---
	m_pPS = GetPS(PS_MASKOBJECT);
	m_pVS = GetVS(VS_OBJECT);
	SetCullMode(CULLMODE_CCW);
}

// ===更新有無===
bool WallPillar::IsUpdate()
{
	return true;
}

// ===衝突処理===
void WallPillar::OnCollision(Object* obj)
{
}

// ===素材のパス設定===
void WallPillar::SetResource(std::string pass)
{
	m_pTex = TEX->Load(pass);		// テクスチャ読込
	if (m_pTex)
		m_ResourcePass = pass;		// パスを設定
}

// ===エディタ表示===
void WallPillar::ImGuiWindow()
{
	// ===テクスチャ選択===
	static int idx = 0;				// 選択中番号

	char combo_preview_value[50];		// 選択中のパス表示用
	strcpy(combo_preview_value, m_ResourcePass.c_str());		// 選択中のパスを表示

	if (ImGui::BeginCombo("Texture", combo_preview_value, ImGuiComboFlags_HeightSmall))
	{
		for (int n = 0; n < MAX_TEX; n++)
		{
			const bool is_selected = (idx == n);								// 選択状態
			if (ImGui::Selectable(GetTexturePass((TEX_TYPE)n), is_selected))	// 変更がある場合
				idx = n;
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				SetResource(GetTexturePass((TEX_TYPE)idx));
			}

		}
		ImGui::EndCombo();
	}
}
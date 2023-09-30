// ===壁クラス実装===
// 壁クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Wall.h"
#include "ModelManager.h"
#include "ShaderList.h"
#include "CollisionManager.h"
#include "TextureManager.h"
#include "ImGuiBase.h"

// ===コンストラクタ===
Wall::Wall() : Collision3D(this)
{
	// ---初期化処理---
	m_transform.Pos = { 0,0,0 };								// 初期座標
	m_transform.Rot = { 0,0,0 };								// 初期回転
	m_transform.Scale = { 1,1,1 };								// 初期拡縮
	m_id = OT_WALL;												// オブジェクトの型設定
	m_bEnable = true;											// 更新・描画可否
	m_col = true;												// 当たり判定有無
	m_bVisible = true;											// 視認可否
	m_Model = MODEL->Load("data/model/NormalWall.fbx");			// モデル読込
	AddCollisionOBB(XMFLOAT3(0, 0, -1), XMFLOAT3(1, 1, 1));		// 当たり判定追加
	m_bStatic = true;											// 静的オブジェクト
	
	m_pPS = GetPS(PS_MASKOBJECT);								// ピクセルシェーダー設定
	m_pVS = GetVS(VS_OBJECT);									// 頂点シェーダー設定

	m_ResourcePass = "data/texture/BlockTex003.png";			// テクスチャパス
	m_pTex = TEX->Load("data/texture/BlockTex003.png");			// テクスチャ読込
}

// ===デストラクタ===
Wall::~Wall()
{
}

// ===更新処理===
void Wall::Update()
{	
	Object3DBase::Update();		// オブジェクト更新
}

// ===描画処理===
void Wall::Draw()
{
	SetCullMode(CULLMODE_CCW);								// 背面カリング
	SetTexturePS(m_pTex, 0);								// テクスチャセット
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// 諧調テクスチャセット
	Object3DBase::Draw();									// 描画処理
}

// ===更新有無===
bool Wall::IsUpdate()
{
	return true;
}

// ===衝突処理===
void Wall::OnCollision(Object* obj)
{
}

// ===素材のパス設定===
void Wall::SetResource(std::string pass)
{
	m_pTex = TEX->Load(pass);
	if (m_pTex)
		m_ResourcePass = pass;
}

// ===エディタ表示===
void Wall::ImGuiWindow()
{
	// ---テクスチャ選択---
	static int idx = 0;						// 選択中番号
	char combo_preview_value[50];			// 選択中のパス表示用
	strcpy(combo_preview_value, m_ResourcePass.c_str());			// 選択中のパスを表示

	if (ImGui::BeginCombo("Texture", combo_preview_value, ImGuiComboFlags_HeightSmall))
	{
		for (int n = 0; n < MAX_TEX; n++)
		{
			const bool is_selected = (idx == n);									// 選択状態
			if (ImGui::Selectable(GetTexturePass((TEX_TYPE)n), is_selected))		// 変更がある場合
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
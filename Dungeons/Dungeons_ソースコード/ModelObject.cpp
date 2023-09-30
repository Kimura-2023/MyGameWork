// ===背景オブジェクトクラス実装===
// 背景オブジェクトクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "ModelObject.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "ImGuiBase.h"

// ===コンストラクタ===
ModelObject::ModelObject()
{
	// ---初期化処理---
	m_transform.Pos = { 0,0,0 };					// 初期座標
	m_transform.Rot = { 0,0,0 };					// 初期回転
	m_transform.Scale = { 1,1,1 };					// 初期拡縮
	m_id = BG_3DOBJ;								// オブジェクト型
	m_bEnable = true;								// 更新・描画可否
	m_col = true;									// 当たり判定有無
	m_bVisible = true;								// 可視状態
	m_Model = MODEL->Load("data/model/Cube.fbx");	// モデル読込
	m_bStatic = true;								// 静的オブジェクト

	m_pPS = GetPS(PS_TOON);							// ピクセルシェーダー設定
	m_pVS = GetVS(VS_OBJECT);						// 頂点シェーダー設定
}

// ===デストラクタ===
ModelObject::~ModelObject()
{
}

// ===更新処理===
void ModelObject::Update()
{
	Object3DBase::Update();						// オブジェクト更新
}

// ===描画処理===
void ModelObject::Draw()
{
	// ---通常描画---
	SetCullMode(CULLMODE_CCW);			// 背面カリング
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");	// 諧調テクスチャ取得
	SetTexturePS(pTex, 1);				// 諧調テクスチャセット
	Object3DBase::Draw();				// 描画

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);			// 前面カリング
	m_pPS = GetPS(PS_OUTLINE);			// 輪郭線描画シェーダーに切替
	m_pVS = GetVS(VS_OUTLINE);			// 輪郭線描画シェーダーに切替
	Object3DBase::Draw();				// 描画


	// ---描画後処理---
	m_pPS = GetPS(PS_TOON);				// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);			// 頂点シェーダ―セット
	SetCullMode(CULLMODE_CCW);			// 背面カリング
}

// ===更新有無===
bool ModelObject::IsUpdate()
{
	return true;
}

// ===素材パス設定===
void ModelObject::SetResource(std::string pass)
{
	m_Model = MODEL->Load(pass);
	if(m_Model)
		m_ResourcePass = pass;
}

// ===エディタ表示===
void ModelObject::ImGuiWindow()
{
	// ===モデル選択===
	static int idx = 0;					// 現在選択番号

	char combo_preview_value[50];							// パス表示用
	strcpy(combo_preview_value,m_ResourcePass.c_str());		// 選択中のパス

	if (ImGui::BeginCombo("Model", combo_preview_value, ImGuiComboFlags_HeightSmall))
	{
		for (int n = 0; n < MAX_MODEL; n++)
		{
			const bool is_selected = (idx == n);								// 選択状態
			if (ImGui::Selectable(GetModelPass((MODEL_TYPE)n), is_selected))	// 選択状態によって判定
			{
				idx = n;														// 選択番号を更新
				SetResource(GetModelPass((MODEL_TYPE)idx));						// 選択したモデルのパスを設定
			}


			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();				// エディタ上の選択更新
			}

		}
		ImGui::EndCombo();
	}
}
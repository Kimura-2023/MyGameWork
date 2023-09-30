// ===ドアクラス実装===
// ドアクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Door.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "MapManager.h"

// ===コンストラクタ===
Door::Door() : Collision3D(this)
{
	// ---初期化処理---
	m_transform.Pos = { 0,0,0 };								// 初期座標
	m_transform.Rot = { 0,0,0 };								// 初期回転
	m_transform.Scale = { 1,1,1 };								// 初期拡縮
	m_id = OT_DOOR;												// オブジェクトの型設定
	m_bEnable = true;											// 更新・描画可否
	m_col = true;												// 当たり判定有無
	m_bVisible = true;											// 視認可否
	m_Model = MODEL->Load("data/model/door.fbx");				// モデル読込
	AddCollisionOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 0.6f));	// 当たり判定追加
	m_bStatic = true;							// 静的オブジェクト

	m_pPS = GetPS(PS_MASKOBJECT);				// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);					// 頂点シェーダーセット
	m_ResourcePass = "data/model/door.fbx";		// 素材へのパス

	m_nDirection = 0;							// 方向初期化
	m_Entrance = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 開始位置初期化
}

// ===デストラクタ===
Door::~Door()
{
}

// ===更新処理===
void Door::Update()
{
	Object3DBase::Update();					// オブジェクト更新
}

// ===描画処理===
void Door::Draw()
{
	SetCullMode(CULLMODE_CCW);								// 背面カリング
	SetTexturePS(TEX->Load("data/texture/ramp.png"), 1);	// 諧調テクスチャセット
	Object3DBase::Draw();									// 描画処理

	// ---輪郭線描画---
	SetCullMode(CULLMODE_CW);		// 前面カリング
	m_pPS = GetPS(PS_OUTLINE);	// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OUTLINE);	// 頂点シェーダーセット
	Object3DBase::Draw();			// 描画処理

	// ---描画設定を元に戻す---
	SetCullMode(CULLMODE_CCW);		// 背面カリング
	m_pPS = GetPS(PS_MASKOBJECT);	// ピクセルシェーダーセット
	m_pVS = GetVS(VS_OBJECT);		// 頂点シェーダーセット
}

// ===更新有無===
bool Door::IsUpdate()
{
	return true;
}

// ===衝突処理===
void Door::OnCollision(Object* obj)
{

}

// ===向き設定===
void Door::SetDirection()
{
	m_nDirection = (int)(m_transform.Rot.y / 90) % 4;			// 実際の向きから方角を算出する

	// ---プレイヤーの出現位置をセット---
	float rad = m_transform.Rot.y * (float)M_PI / 180.0f;		// 補正する方向
	m_Entrance.x = 3.0f * -sinf(rad) + m_transform.Pos.x;		// 補正する値x方向
	m_Entrance.z = 3.0f * -cosf(rad) + m_transform.Pos.z;		// 補正する値z方向
	m_Entrance.y = m_transform.Pos.y;							// 高さはそのまま
}

// ===ドア開放===
void Door::Open()
{
	m_Model = MODEL->Load("data/model/OpenDoor.fbx");			// モデルをドア開放後のモノに置き換える
}
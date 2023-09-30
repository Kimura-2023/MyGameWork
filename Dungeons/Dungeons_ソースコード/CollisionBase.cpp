// ===当たり判定ベースクラス実装===
// 当たり判定ベースクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "CollisionBase.h"
#include "DebugProc.h"

// ===コンストラクタ===
CollisionBase::CollisionBase()
{
	// ---初期化処理---
	m_Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);							// ワールド座標
	m_Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);							// ローカル回転
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);						// ローカルスケール
	m_Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);						// ローカル座標
	m_pParent = nullptr;										// 当たり判定の大元
	XMStoreFloat4x4(&m_World,XMMatrixIdentity());				// ワールドマトリックス
	m_coltype = COL_NONE;										// 当たり判定種類
	m_Enable = true;											// 当たり判定有無
	m_Visible = false;											// デバッグ表示有無
	m_bHit = false;												// 衝突したかどうか
	Init();														// デバッグ表示のマテリアル情報初期化
}

// ===デストラクタ===
CollisionBase::~CollisionBase()
{
}

// ===マテリアル初期化===
void CollisionBase::Init()
{
	m_Material.m_Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);
	m_Material.m_Ambient = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);
	m_Material.m_Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_Power = 1.0f;
}

// ===更新===
void CollisionBase::Update()
{
	// ---デバッグ表示変更処理---
	if(m_bHit)		// 衝突時
		m_Material.m_Diffuse = m_Material.m_Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f);		// 赤色
	else			// 非衝突時
		m_Material.m_Diffuse = m_Material.m_Ambient = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f);		// 緑色

	m_bHit = false;			// 当たり判定状態更新
	UpdateMatrix();			// 行列計算
	m_Pos.x = m_World._41;	// x座標設定
	m_Pos.y = m_World._42;	// y座標設定
	m_Pos.z = m_World._43;	// z座標設定
}

void CollisionBase::UpdateMatrix()
{
	// ---ローカル変数---
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;
	mtxWorld = XMMatrixIdentity();	// 初期化

	XMMATRIX parent = XMLoadFloat4x4(&m_pParent->GetWorld());		// 親行列取得

	// ---回転を反映---
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));		// 回転行列計算
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);					// ワールド変換行列に反映

	// ---移動を反映---
	mtxTranslate = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);		// 移動行列計算
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);			// ワールド変換行列に反映

	// ---親からのワールド変換も反映させる---
	mtxWorld = mtxWorld * parent;

	// ---ワールドマトリックス設定---
	XMStoreFloat4x4(&m_World, mtxWorld);
}

// ===中心座標取得===
XMFLOAT3& CollisionBase::GetCenter()
{
	return m_Pos;
}

// ===ワールドマトリックス取得===
XMFLOAT4X4& CollisionBase::GetWorld()
{
	return m_World;
}

// ===有効かどうか取得===
bool CollisionBase::GetEnable()
{
	return m_Enable;
}

// ===有効可否===
void CollisionBase::SetEnable(bool flg)
{
	m_Enable = flg;
}

// ===当たり判定の種類取得===
COLTYPE CollisionBase::GetColType()
{
	return m_coltype;
}

// ===可視状態取得===
bool CollisionBase::GetVisible()
{
	return m_Visible;
}

// ===可視状態セット===
void CollisionBase::SetVisible(bool flg)
{
	m_Visible = flg;
}

// ===親セット===
void CollisionBase::SetParent(Collision3D* col)
{
	m_pParent = col;
}

// ===親取得===
Collision3D* CollisionBase::GetFollowCollision()
{
	return m_pParent;
}

// ===衝突状態取得===
bool CollisionBase::GetHit()
{
	return m_bHit;
}
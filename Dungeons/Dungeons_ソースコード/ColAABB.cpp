// ===当たり判定(AABB)クラス実装===
// 当たり判定(AABB)クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "ColAABB.h"
#include "Light.h"
#include "ModelManager.h"
#include "DebugProc.h"
#include "ColSphere.h"

// ===静的メンバ===
CBox* CollAABB::m_pBox = nullptr;			// デバッグ表示用メッシュ
int CollAABB::m_nCnt = 0;					// AABB総数

// ===コンストラクタ(オフセットなし)===
CollAABB::CollAABB(Collision3D* col, XMFLOAT3 size)
{
	// ---初期化処理---
	Init();									// 初期化
	m_Size = size;							// サイズをセット
	SetParent(col);							// 当たり判定の元をセット
	HRESULT hr = m_pBox->Init(m_Size);		// メッシュ初期化

	// ---例外処理---
	if (FAILED(hr))
	{
		m_pBox->Fin();			// デバッグ描画終了
		SAFE_DELETE(m_pBox);	// 解放
		return;
	}
	m_pBox->SetMaterial(&m_Material);		// マテリアルセット
}

// ===コンストラクタ(オフセットあり)===
CollAABB::CollAABB(Collision3D* col, XMFLOAT3 off, XMFLOAT3 size)
{
	// ---初期化処理---
	Init();									// 初期化
	m_Offset = off;							// オフセットをセット
	m_Size = { size.x / 2,size.y / 2,size.z / 2 };		// サイズをセット
	SetParent(col);							// 親をセット
	HRESULT hr = m_pBox->Init(m_Size);		// メッシュ初期化

	// ---例外処理---
	if (FAILED(hr))
	{
		m_pBox->Fin();			// デバッグ描画終了
		SAFE_DELETE(m_pBox);	// 解放
		return;
	}
	m_pBox->SetMaterial(&m_Material);		// マテリアルセット
}

// ===デストラクタ===
CollAABB::~CollAABB()
{
	// ---終了処理---
	--m_nCnt;					// 当たり判定総数減少
	if (m_nCnt <= 0)
	{
		m_pBox->Fin();			// デバッグ表示終了
		SAFE_DELETE(m_pBox);	// 解放
	}
}

// ===初期化処理===
void CollAABB::Init()
{
	// ---初期化---
	m_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);			// 初期サイズ
	m_coltype = COL_AABB;							// 当たり判定型

	if (m_nCnt == 0)
	{
		m_pBox = new CBox();						// メッシュ生成
	}
	m_nCnt++;										// AABB総数加算
}

// ===ヒット確認===
bool CollAABB::CheckHit(CollisionBase* col)
{
	// ---除外処理---
	if (col->GetFollowCollision() == GetFollowCollision())
		return false;
	if (!col->GetEnable())
		return false;

	// ---ヒット確認---
	XMFLOAT3 min, max;															// ボリューム計算
	min = { m_Pos.x - m_Size.x ,m_Pos.y - m_Size.y ,m_Pos.z - m_Size.z };
	max = { m_Pos.x + m_Size.x ,m_Pos.y + m_Size.y ,m_Pos.z + m_Size.z };

	XMFLOAT3 pos = col->GetCenter();											// 相手座標

	switch (col->GetColType())
	{
	case COL_AABB: {													// ---AABB同士の判定
		XMFLOAT3 size = ((CollAABB*)col)->GetSize();
		XMFLOAT3 omin, omax;
		omin = { pos.x - size.x ,pos.y - size.y ,pos.z - size.z };		// 相手最小
		omax = { pos.x + size.x ,pos.y + size.y ,pos.z + size.z };		// 相手最大

		m_bHit = false;						// ヒット状態を更新

		// ---非衝突時---
		if (min.x > omax.x) return false;
		if (max.x < omin.x) return false;
		if (min.y > omax.y) return false;
		if (max.y < omin.y) return false;
		if (min.z > omax.z) return false;
		if (max.z < omin.z) return false;

		// ---衝突時---
		CDebugProc::Print("ﾋｯﾄ\n");			// デバッグ表示
		m_bHit = true;						// ヒット状態を更新
		return true;						
		break;
	}
	case COL_NONE:
		break;
	default:
		break;
	}
	return false;
}

// ===デバッグ表示===
void CollAABB::Draw()
{
	// ---除外処理---
	if (!GetVisible())
		return;

	// ---描画処理---
	m_pBox->SetMaterial(&m_Material);		// マテリアルをセット
	SetBlendState(BS_ALPHABLEND);			// ブレンドステート設定

	m_pBox->SetWorld(&GetWorld());			// ワールド変換行列をセット
	m_pBox->Draw();							// デバッグ表示

	SetBlendState(BS_NONE);					// ブレンドステート設定
}

// ===AABBサイズ取得===
XMFLOAT3 CollAABB::GetSize()
{
	return m_Size;
}
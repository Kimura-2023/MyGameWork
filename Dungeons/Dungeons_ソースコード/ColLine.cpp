// ===当たり判定(線分)クラス実装===
// 当たり判定(線分)クラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "ColLine.h"
#include "Light.h"
#include "DebugProc.h"
#include "ColSphere.h"
#include "Calc.h"

// ===静的メンバ===
Line* CollLine::m_Line = nullptr;				// デバッグ表示用
int CollLine::m_nCnt = 0;						// 総数

// ===コンストラクタ(オフセットなし)===
CollLine::CollLine(Collision3D* col, XMFLOAT3 end)
{
	// ---初期化処理---
	Init();							// 初期化
	m_vec = CalcUnitVector(end);	// ベクトル設定
	m_End = end;					// 終点座標設定
	SetParent(col);					// 親をセット

	m_Line->AddLine(m_Pos, m_End);	// 線分設定
}

// ===コンストラクタ(オフセットあり)===
CollLine::CollLine(Collision3D* col, XMFLOAT3 off, XMFLOAT3 end)
{
	// ---初期化処理---
	Init();							// 初期化
	m_Offset = off;					// オフセット設定
	m_Pos.x += m_Offset.x;			// 中心座標(x)更新
	m_Pos.y += m_Offset.y;			// 中心座標(y)更新
	m_Pos.z += m_Offset.z;			// 中心座標(z)更新

	m_vec = CalcUnitVector(end);	// ベクトル設定
	m_End = end;					// 終点座標設定
	SetParent(col);					// 親をセット

	m_Line->AddLine(m_Pos, m_End);	// 線分設定
}

// ===デストラクタ===
CollLine::~CollLine()
{
	--m_nCnt;						// 当たり判定総数減少
	if (m_nCnt <= 0)
	{
		m_Line->Fin();				// デバッグ表示終了
		SAFE_DELETE(m_Line);		// 解放
	}
}

// ===初期化処理===
void CollLine::Init()
{
	// ---初期化---
	m_vec = m_End = XMFLOAT3(1.0f, 1.0f, 1.0f);		// 終点座標、ベクトルセット
	m_Pos = GetCenter();			// 中心座標セット
	m_coltype = COL_LINE;			// 当たり判定型設定

	if (m_nCnt == 0)
	{
		m_Line = new Line();		// 生成
	}
	m_nCnt++;						// 当たり判定総数加算
}

// ===ヒット確認===
bool CollLine::CheckHit(CollisionBase* col)
{
	// ---除外処理---
	if (col->GetFollowCollision() == GetFollowCollision())
		return false;
	if (!col->GetEnable())
		return false;

	// ---ヒット確認---
	XMFLOAT3 pos = col->GetCenter();											// 相手座標

	switch (col->GetColType())			// 相手当たり判定によって処理を切替
	{
	case COL_SPHERE: {					// ---境界球

		XMFLOAT3 vline = GetVec();					// 線分のベクトルを算出
		float xa = m_Pos.x - pos.x;
		float ya = m_Pos.y - pos.y;
		float za = m_Pos.z - pos.z;
		float rad = ((CollSphere*)col)->GetRadius();

		float a = vline.x * vline.x + vline.y * vline.y + vline.z * vline.z;
		float b = 2.0f * (vline.x * xa + vline.y * ya + vline.z * za);
		float c = (xa * xa) + (ya * ya) + (za * za) - (rad * rad);

		// ---場合分けをする---
		// 「b^2 - 4ac」の値が0以下の場合は衝突なし
		float d = (b * b) - (4.0f * a * c);			// b^2 - 4ac
		if (d < 0)
		{
			m_bHit = false;
			return false;
		}

		// ---tの値(2種類)を求める---
		d = sqrtf(d);
		float t1 = (-b + d) / (2.0f * a);
		float t2 = (-b - d) / (2.0f * a);

		// ---線分の最寄り(最小値)を設定---
		float t = 2.0f;
		if (t1 >= 0.0f && t1 <= 1.0f && t1 < t) {
			t = t1;
		}
		if (t2 >= 0.0f && t2 <= 1.0f && t2 < t) {
			t = t2;
		}

		// ---衝突なし---
		if (t > 1.0f)
		{
			m_bHit = false;
			return false;
		}

		// ---衝突---
		CDebugProc::Print("ﾋｯﾄ\n");
		m_bHit = true;
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
void CollLine::Draw()
{
	// ---除外処理---
	if (!GetVisible())
		return;

	// ---描画処理---
	m_Line->SetWorldMatrix(GetWorld());				// ワールド変換行列をセット
	m_Line->SetMaterial(m_Material.m_Diffuse);		// マテリアルをセット
	m_Line->DrawLines();							// 線分描画
}

// ===終点座標取得===
XMFLOAT3 CollLine::GetEnd()
{
	return m_End;
}

// ===ベクトル取得===
XMFLOAT3 CollLine::GetVec()
{
	// ---終点の座標を更新(中心座標と線分ベクトルから終点を求める)---
	m_End.x = m_Pos.x + m_vec.x;
	m_End.y = m_Pos.y + m_vec.y;
	m_End.z = m_Pos.z + m_vec.z;

	// ---ベクトルを返す---
	return XMFLOAT3(m_End.x - m_Pos.x, m_End.y - m_Pos.y, m_End.z - m_Pos.z);
}
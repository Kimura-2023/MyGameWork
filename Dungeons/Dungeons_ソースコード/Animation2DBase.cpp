// ===2Dアニメーションベースクラス実装===
// 2Dアニメーションベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Animation2DBase.h"
#include "ObjectManager.h"
#include "DebugProc.h"

// ===コンストラクタ===
Animation2DBase::Animation2DBase()
{
	// ---初期化処理---
	m_fdefault = 30.0f;
	m_fSpd = m_fdefault;
	m_nFrame = 0;
	m_nOld = m_nFrame;
	m_nStart = 0;
	m_nAnimCnt = m_TexCut.x * m_TexCut.y;

	m_Loop = false;
	m_bDestroy = false;

	m_nFrame = 0;								// 再生中フレーム数
	m_nOld = m_nFrame;							// 前フレーム
	m_nStart = 0;								// アニメーションの開始uv
	m_nAnimCnt = m_TexCut.x * m_TexCut.y;		// アニメーションする枚数
	m_fdefault = 30.0f;
	m_fSpd = m_fdefault;		// 再生スピード(計算用)

	m_Loop = false;				// 再生し続ける
	m_bEnd = false;				// 再生終了フラグ
	m_bDestroy = false;			// 再生終了で破棄するかどうか
}

// ===デストラクタ===
Animation2DBase::~Animation2DBase()
{
}

//===更新処理===
void Animation2DBase::Update()
{
	m_bEnd = false;
	m_fSpd--;				// 再生スピード更新

	// ---再生判定---
	if (m_fSpd < 0)
	{
		// ---uv更新---
		if (m_nFrame != m_nOld)
		{
			m_uv.x = (float)(m_nFrame % (int)m_TexCut.x) / m_TexCut.x;
			m_uv.y = (float)(m_nFrame / (int)m_TexCut.x) / m_TexCut.y;
			m_nOld = m_nFrame;
		}
		m_nFrame++;															// アニメーションフレーム加算
		m_fSpd = m_fdefault;												// 再生秒数リセット

		// ---再生終了---
		if (m_nFrame % (m_nStart + m_nAnimCnt) == 0)
		{
			m_bEnd = true;					// 終了フラグ
			if (m_Loop)						// ループ判定
				m_nFrame = m_nStart;
			else if (m_bDestroy)			// 破棄判定
				OBJECT->Release(this);		// オブジェクトを破棄
			else
			{
				m_nFrame = m_nStart;		// 最初のフレームに移動
				m_bEnable = false;			// 描画・更新停止
			}
		}
	}
}

// ===再生スピードセット===
void Animation2DBase::SetSpeed(float f)
{
	m_fdefault = f;
}

// ===アニメーション枚数セット===
void Animation2DBase::SetMaxAnim(int n)
{
	m_nAnimCnt = n;
}

// ===開始フレームセット===
void Animation2DBase::SetStartFrame(int n)
{
	m_nStart = n;
	m_nFrame = m_nStart;
}

// ===アニメーション終了判定===
bool Animation2DBase::GetEnd()
{
	return m_bEnd;
}

// ===現在再生フレーム取得===
int Animation2DBase::GetFrame()
{
	return m_nFrame;
}

// ===アニメーション終了セット===
void Animation2DBase::SetnEnd()
{
	m_bEnd = false;
}
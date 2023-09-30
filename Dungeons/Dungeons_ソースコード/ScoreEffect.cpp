// ===スコアエフェクトクラス実装===
// スコアエフェクトクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "ScoreEffect.h"
#include "Polygon.h"
#include "TextureManager.h"
#include "Calc.h"

// ===定数・マクロ定義===
#define TEX_WIDTH (56.0f)
#define TEX_HEIGHT (48.0f)

// ===コンストラクタ===
ScoreEffect::ScoreEffect()
{
	// ---初期化処理---
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期座標
	m_transform.Scale = XMFLOAT3(0.7f, 0.6f, 1.0f);		// サイズ
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv左上座標
	m_TexCut = XMFLOAT2(1, 1);							// uv右上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// 色
	m_id = BILL_SCORE;									// オブジェクトタグ
	m_bBillboard = true;								// ビルボード設定
	m_Blend = BS_ALPHABLEND;							// ブレンド設定
	m_Texture = TEX->Load("data/texture/Number.png");	// テクスチャ
	m_nScore = 0;
}

// ===デストラクタ===
ScoreEffect::~ScoreEffect()
{
}

// ===更新処理===
void ScoreEffect::Update()
{
	// ---更新判定---
	if (!m_bEnable)
		return;

	// ---状態を更新---
	m_color.w -= 0.01f;					// α値を徐々に下げる
	//m_transform.Pos.y += 0.01f;			// 移動速度を設定

	if (m_color.w <= 0.0f)				// 透明になったら
	{
		m_bEnable = false;		// 更新可否を設定
		m_color.w = 1.0f;		// α値を元に戻す
	}

}

// ===描画処理===
void ScoreEffect::Draw()
{
	// ---描画判定
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---描画準備---
	SetZWrite(false);													// 深度バッファ更新可否セット
	ID3D11DeviceContext* pDC = GetDeviceContext();						// デバイスコンテキスト取得
	SetBlendState(m_Blend);												// ブレンド情報をセット
	CPolygon::SetColor(m_color);										// 色をセット
	CPolygon::SetDraw(!m_bBillboard);									// UI判定をfalse
	CPolygon::SetBill(m_bBillboard);									// ビルボード設定
	CPolygon::SetTexture(m_Texture);									// テクスチャをセット	
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);		// サイズセット
	CPolygon::SetFrameSize(1.0f / 10.0f, 0.5f);							// 右下uv座標セット

	// ---スコア表示---
	int score = m_nScore;
	int digit = 0;														// 桁数
	Digit(&digit, score);												// 桁数を算出
	for (int i = 0; i < digit; ++i)
	{
		int num = (score / (int)pow(10, digit - (i + 1))) % 10;			// スコアを一桁ずつ描画するために桁ごとの値を算出
		CPolygon::SetPos(m_transform.Pos.x + m_transform.Scale.x / 2 * i, m_transform.Pos.y, m_transform.Pos.z);		// 座標をセット
		CPolygon::SetUV((float)num / 10.0f, 0.0f);													// 左上uv座標セット
		CPolygon::Draw(pDC);																		// 数字を描画
	}

	CPolygon::SetBill(!m_bBillboard);		// ビルボード設定を初期化
	CPolygon::SetDraw(m_bBillboard);		// UI描画設定
	SetZWrite(true);			// 深度バッファ更新可否セット
	SetZBuffer(true);			// 深度バッファ有効化
}

// ===スコア設定===
void ScoreEffect::SetScore(const int& num)
{
	m_nScore = num;
}
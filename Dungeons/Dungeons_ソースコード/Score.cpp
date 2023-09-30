// ===スコアUIクラス実装===
// スコアUIクラスを実装
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Score.h"
#include "Polygon.h"
#include "TextureManager.h"
#include "ScoreManager.h"
#include "Calc.h"

// ===定数・マクロ定義===
#define TEX_WIDTH (56.0f)
#define TEX_HEIGHT (48.0f)

// ===コンストラクタ===
Score::Score()
{
	// ---初期化処理---
	m_transform.Pos = XMFLOAT3(-SCREEN_WIDTH / 2 + TEX_WIDTH, -SCREEN_HEIGHT / 2 + TEX_HEIGHT, 0.0f);		// 初期座標
	m_transform.Scale = XMFLOAT3(TEX_WIDTH, TEX_HEIGHT, 1.0f);					// サイズ
	m_uv = XMFLOAT2(0.0f, 0.0f);						// uv左上座標
	m_TexCut = XMFLOAT2(1, 1);							// uv右上座標
	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);			// 色
	m_id = UI_SCORE;									// オブジェクトタグ
	m_Texture = TEX->Load("data/texture/Number.png");	// テクスチャ
}

// ===デストラクタ===
Score::~Score()
{
}

// ===更新処理===
void Score::Update()
{
}

// ===描画処理===
void Score::Draw()
{
	// ---描画判定
	if (!m_bEnable)
		return;
	if (m_Texture == nullptr)
		return;

	// ---文字描画---
	SetZWrite(false);													// 深度バッファ更新可否セット
	ID3D11DeviceContext* pDC = GetDeviceContext();						// デバイスコンテキスト取得
	SetBlendState(m_Blend);												// ブレンド情報をセット
	CPolygon::SetColor(m_color);										// 色をセット
	if (SCENE->GetCurrentScene()->GetSceneName() == "GameScene")
		CPolygon::SetTexture(TEX->Load("data/texture/score.png"));			// テクスチャをセット
	else
		CPolygon::SetTexture(TEX->Load("data/texture/hiscore.png"));		// テクスチャをセット

	CPolygon::SetSize(240.0f, m_transform.Scale.y / 2);					// サイズセット
	CPolygon::SetRotate(m_transform.Rot);								// 回転セット				
	CPolygon::SetPos(m_transform.Pos.x + TEX_WIDTH, m_transform.Pos.y + TEX_HEIGHT);	// 座標をセット
	CPolygon::SetUV(0.0f, 0.0f);												// 左上uv座標セット
	CPolygon::SetFrameSize(1.0f, 1.0f);											// 右下uv座標セット
	CPolygon::Draw(pDC);														// 描画処理

	// ---描画準備---
	CPolygon::SetTexture(m_Texture);									// テクスチャをセット	
	CPolygon::SetSize(m_transform.Scale.x, m_transform.Scale.y);					// サイズセット

	// ---スコア表示---
	int score;
	if (SCENE->GetCurrentScene()->GetSceneName() == "GameScene")		// シーンによって処理を切替
		score = SCORE->GetScore();										// 現在スコア取得
	else
		score = SCORE->GetHiScore();									// ハイスコアを取得
	int digit = 0;														// 桁数
	Digit(&digit, score);												// 桁数を算出
	for (int i = 0; i < digit; ++i)
	{
		int num = (score / (int)pow(10, digit - (i + 1))) % 10;
		CPolygon::SetPos(m_transform.Pos.x + m_transform.Scale.x / 2 * i, m_transform.Pos.y);		// 座標をセット
		CPolygon::SetUV((float)num / 10.0f, 0.0f);													// 左上uv座標セット
		CPolygon::SetFrameSize(1.0f / 10.0f, 0.5f);													// 右下uv座標セット
		CPolygon::Draw(pDC);																		// 数字を描画
	}

	SetZWrite(true);			// 深度バッファ更新可否セット
	SetZBuffer(true);			// 深度バッファ有効化
}
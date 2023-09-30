// フェード イン/アウト [Fade.cpp]
#include "Fade.h"
#include "Polygon.h"
#include "Sound.h"

// 静的メンバ
EFade CFade::m_fade = FADE_NONE;
XMFLOAT4 CFade::m_vColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
float CFade::m_fRate = 1.0f / (60.0f * 0.5f);

// 初期化
void CFade::Init()
{
	m_vColor.w = 1.0f;			// 不透明度
	m_fade = FADE_IN;			// 状態
}

// 更新
void CFade::Update()
{
	// 何もしていない
	if (m_fade == FADE_NONE) {
		return;
	}

	// フェードアウト処理
	if (m_fade == FADE_OUT) {
		m_vColor.w += m_fRate;	// 不透明度を増加
		if (m_vColor.w >= 1.0f) {
			// フェード終了
			m_vColor.w = 1.0f;
			m_fade = FADE_NONE;
		}
		// 音もフェードアウト
		CSound::SetVolume(1.0f - m_vColor.w);
		return;
	}

	// フェードイン処理
	if (m_fade == FADE_IN) {
		m_vColor.w -= m_fRate;	// 不透明度を下げる
		if (m_vColor.w <= 0.0f) {
			// フェード終了
			m_vColor.w = 0.0f;
			m_fade = FADE_NONE;
		}
		// 音もフェードイン
		CSound::SetVolume(1.0f - m_vColor.w);
	}
}

// 描画
void CFade::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(m_vColor);
	CPolygon::SetTexture(nullptr);
	CPolygon::SetRotate(XMFLOAT3(0.0f, 0.0f, 0.0f));
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// フェードアウト開始
void CFade::Out(float fSecond)
{
	if (m_fade == FADE_NONE) {
		m_fade = FADE_OUT;
		m_fRate = 1.0f / (60.0f * fSecond);
	}
}

// フェードイン開始
void CFade::In(float fSecond)
{
	if (m_fade == FADE_NONE) {
		m_fade = FADE_IN;
		m_fRate = 1.0f / (60.0f*fSecond);
	}
}

// フェードイン アウト カラー設定
void CFade::SetColor(XMFLOAT3 vColor)
{
	m_vColor.x = vColor.x;
	m_vColor.y = vColor.y;
	m_vColor.z = vColor.z;
}

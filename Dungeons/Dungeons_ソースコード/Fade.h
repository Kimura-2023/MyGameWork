// フェード イン/アウト [Fade.h]
#pragma once
#include "main.h"

// 定数定義
enum EFade {
	FADE_NONE = 0,	// 何もしていない
	FADE_IN,		// フェードイン処理
	FADE_OUT,		// フェードアウト処理

	MAX_FADE
};

// クラス定義
class CFade {
private:
	static EFade m_fade;
	static XMFLOAT4 m_vColor;
	static float m_fRate;

public:
	static void Init();
	static void Update();
	static void Draw();

	static void Out(float fSecond = 1.0f);
	static void In(float fSecond = 1.0f);
	static EFade GetState() { return m_fade; }
	static void SetColor(XMFLOAT3 vColor);
};

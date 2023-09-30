#pragma once

// ===エフェクト管理クラス定義===
// エフェクト管理クラスを定義
// Author Kimura
// ==============================

// ===インクルード===
#include "main.h"
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

// ---ライブラリのリンク
#ifdef _DEBUG
#pragma comment(lib, "Effekseer_2017_d.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017_d.lib")
#else
#pragma comment(lib, "Effekseer_2017.lib")
#pragma comment(lib, "EffekseerRendererDX11_2017.lib")
#endif

// ===定数・マクロ定義===
#define EFFECT (EffectManager::Get())				// インスタンス取得
#define MAX_PARTICLE (1000)							// パーティクル最大数

// ===エフェクト種類定義===
enum EFFECT_TYPE
{
	HIT_EFFECT = 0,
	MOVE_EFFECT,
	SWORD_EFFECT,
	KICK_EFFECT,
	EXPLOSION_EFFECT,
	ENEMY_EFFECT,
	ENEMY_DEAD_EFFECT,
	REPEL_EFFECT,
	AVOID_EFFECT,

	EFFECT_MAX,
};

// ===クラス定義===
class EffectManager
{
	// ---メンバ関数---
public:
	EffectManager();					// コンストラクタ
	~EffectManager();					// デストラクタ

	static EffectManager* Get();		// インスタンス取得
	static void Destroy();				// インスタンス破棄
	static void Create();				// インスタンス生成

	void Init();						// 初期化処理
	void LoadAll();						// エフェクト一括読込

	void Update();						// 更新処理
	void Release();						// エフェクト全解放
	void Draw();						// 描画処理

	void Play(EFFECT_TYPE,const XMFLOAT3& pos);			// エフェクト再生

	// ---メンバ変数---
private:
	static EffectManager* m_pInstance;						// インスタンス

	EffekseerRendererDX11::RendererRef m_renderer;			// 描画用
	Effekseer::ManagerRef m_manager;						// 再生管理
	Effekseer::Handle m_handle;								// 
	std::vector<Effekseer::EffectRef> m_EffectResource;		// エフェクトリソース
};
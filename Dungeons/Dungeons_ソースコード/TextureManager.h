#pragma once

// ===テクスチャ管理クラス定義===
// テクスチャ管理クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include "main.h"
#include "Object2DBase.h"
#include <vector>

// ===定数・マクロ定義===
#define TEX (TextureManager::Get())			// インスタンス取得

// ===テクスチャ種類定義===
enum TEX_TYPE
{
	WALL_TEX1 = 0,		// 壁テクスチャ1
	WALL_TEX2,			// 壁テクスチャ2
	WALL_TEX3,			// 壁テクスチャ3
	EFF_TEX1,			// 円パーティクル
	TITLE_TEX1,			// タイトル1
	TITLE_TEX2,			// タイトル2
	TITLE_TEX3,			// タイトル3
	GAMEOVER_TEX1,		// ゲームオーバー1
	GAMECLEAR_TEX1,		// ゲームオーバー2
	RAMP_TEX,			// トゥーンシェーダー用
	STAGE_TEX1,			// ステージ
	WALL_TEX4,			// 壁テクスチャ4

	MAX_TEX
};

// ===クラス定義===
class TextureManager
{
	// ---メンバ関数
public:
	TextureManager();
	~TextureManager();

	static TextureManager* Get();			// インスタンス取得
	static void Destroy();					// インスタンス破棄
	static void Create();					// インスタンス生成

	void Release();							// テクスチャ解放

	ID3D11ShaderResourceView* Load(std::string);		// 読込
	void Draw();										// 描画処理

	// ---メンバ変数
private:
	static TextureManager* m_pInstance;						// インスタンス
	std::map<std::string, Texture*> m_TextureResource;		// リソースリスト
};

// ===関数定義===
LPCSTR GetTexturePass(TEX_TYPE);	// imgui上で使用するテクスチャデータのファイルパス
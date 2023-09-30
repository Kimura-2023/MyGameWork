// ===配置2Dオブジェクトベースクラス Object2DBase.h
// ---Authot kimura
#pragma once

// ===インクルード===
#include "main.h"
#include "Object.h"

// ===クラス定義===
class Object2DBase : public Object
{
	// ---メンバ関数---
public:
	Object2DBase();				// コンストラクタ
	virtual ~Object2DBase();	// デストラクタ

	void Init();				// 初期化
	virtual void Update();		// 更新
	virtual void Draw();		// 描画

	void SetResource(std::string);		// 素材パス設定

	void SetUseBlendState(EBlendState);	// ブレンドステートをセット
	void SetColor(XMFLOAT4 c) { m_color = c; }		// 色セット

	XMFLOAT3 GetRotate() { return m_transform.Rot; }		// 回転取得
	XMFLOAT4 GetColor() { return m_color; }					// 色取得
	XMFLOAT2 GetScale() { return m_Scale; }					// 拡縮取得
	XMFLOAT2 GetUV() { return m_uv; }						// 左上UV取得
	XMFLOAT2 GetTexSize();									// 右下UV取得

	float GetCamVecSize();							// カメラとの距離

	bool GetUI() { return m_bui; }					// UIかどうか
	bool GetBill() { return m_bBillboard; }			// ビルボード有無
	EBlendState GetBlendState() { return m_Blend; }	// ブレンドステート取得

	void UpdateMatrix();		// ワールドマトリクス更新

	virtual void ImGuiWindow() { ; }

	// ---メンバ変数---
protected:
	XMFLOAT2 m_Scale;		// 拡縮
	XMFLOAT2 m_uv;			// 左上uv座標
	XMFLOAT2 m_TexCut;		// テクスチャ分割数
	XMFLOAT4 m_color;		// 色

	bool m_bBillboard;		// ビルボード有無
	bool m_bui;

	EBlendState m_Blend;	// ブレンドステート
	ID3D11ShaderResourceView* m_Texture;		// テクスチャ
private:
	float CalcObjSize(XMFLOAT2*);
};
// 2Dポリゴン処理クラス定義 [Polygon.h]

#pragma once
#include "main.h"
#include "ShaderList.h"	

class CPolygon
{
private:
	static ID3D11ShaderResourceView* m_pTexture;	// テクスチャへのポインタ

	static VERTEX_2D m_vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	static XMFLOAT3 m_vPos;							// ポリゴンの移動量
	static XMFLOAT3 m_vAngle;						// ポリゴンの回転量
	static XMFLOAT3 m_vScale;						// ポリゴンのサイズ
	static XMFLOAT4 m_vColor;						// ポリゴンの頂点カラー
	static bool m_bInvalidate;						// 頂点データ更新フラグ

	static XMFLOAT2 m_vPosTexFrame;					// UV座標
	static XMFLOAT2 m_vSizeTexFrame;				// テクスチャ抽出サイズ

	static ID3D11Buffer* m_pConstantBuffer;			// 定数バッファ
	static ID3D11Buffer* m_pVertexBuffer;			// 頂点バッファ
	static ID3D11SamplerState* m_pSamplerState;		// テクスチャ サンプラ

	static XMFLOAT4X4 m_mProj;						// 射影変換行列
	static XMFLOAT4X4 m_mView;						// ビュー変換行列
	static XMFLOAT4X4 m_mWorld;						// ワールド変換行列
	static XMFLOAT4X4 m_mTex;						// テクスチャ変換行列

	static VertexShader* m_VtxShader;				// 頂点シェーダー
	static PixelShader* m_PixShader;				// ピクセルシェーダー

	static bool m_bUI;								// オブジェクトか否か	
	static bool m_bBillboard;						// ビルボード有無

private:
	static HRESULT MakeVertex(ID3D11Device* pDevice);
	static void SetVertex(void);

public:
	static HRESULT Init(ID3D11Device* pDevice);
	static void Fin();
	static void Draw(ID3D11DeviceContext* pDeviceContext);
	static void SetTexture(ID3D11ShaderResourceView* pTexture);
	static void SetPos(float fX, float fY);
	static void SetPos(float fX, float fY, float fZ);
	static void SetPos(XMFLOAT3 vPos);
	static void SetSize(float fScaleX, float fScaleY);
	static void SetAngle(float fAngle);
	static void SetRotate(XMFLOAT3 vRot);
	static void SetUV(float fU, float fV);
	static void SetFrameSize(float fWidth, float fHeight);
	static void SetColor(float fRed, float fGreen, float fBlue);
	static void SetColor(XMFLOAT3 vColor);
	static void SetAlpha(float fAlpha);
	static void SetColor(float fRed, float fGreen, float fBlue, float fAlpha);
	static void SetColor(XMFLOAT4 vColor);

	static void SetDraw(bool);
	static void SetBill(bool);
};

#pragma once
#include "main.h"
#include "DXBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include <vector>

class Line
{
public:
	Line();
	~Line();
	static HRESULT InitLine();									// 線描画用シェーダー等初期化		

	HRESULT InitVertexBuffer();									// 頂点バッファ作成
	void AddLine(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f});			// 線追加
	void DrawLines();							// 線描画

	void SetWorldMatrix(const XMFLOAT4X4&);
	void SetMaterial(const XMFLOAT4&);

	static void Fin();
private:
	static ConstantBuffer* m_pCBuffer;
	static VertexShader* m_pVertex;
	static  PixelShader* m_pPixel;

	DXBuffer m_Lines;
	std::vector<VERTEX> m_LineData;				// 線情報
	XMFLOAT4X4 m_World;							// ワールド変換行列
};
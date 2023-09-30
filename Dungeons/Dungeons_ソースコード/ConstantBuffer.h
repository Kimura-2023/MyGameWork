// ---定数バッファクラス
#pragma once
#include "main.h"

class ConstantBuffer
{
public:
	ConstantBuffer();					// コンストラクタ
	~ConstantBuffer();					// デストラクタ

	HRESULT Create(UINT size);			// 定数バッファ生成(データサイズ(4バイト区切り))
	void Write(const void* pData);		// 定数バッファ書き換え
	void BindVS(UINT slot);				// 頂点シェーダーのregister()に定数バッファをバインド
	void BindPS(UINT slot);				// ピクセルシェーダーのregister()に定数バッファをバインド

private:
	ID3D11Buffer* m_pBuffer;			// バッファ
};
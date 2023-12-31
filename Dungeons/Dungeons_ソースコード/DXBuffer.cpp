#include "DXBuffer.h"

DXBuffer::DXBuffer()
	:m_pVtxBuffer(NULL),m_vtxCount(0),m_vtxSize(0)
	,m_pIdxBuffer(NULL),m_idxCount(0),m_idxSize(0)
{
}
DXBuffer::~DXBuffer()
{
	SAFE_RELEASE(m_pIdxBuffer);
	SAFE_RELEASE(m_pVtxBuffer);
}

HRESULT DXBuffer::Create(const Desc& desc)
{
	HRESULT hr = E_FAIL;
	hr = CreateVertexBuffer(desc.pVtx, desc.vtxSize, desc.vtxCount, desc.isWrite);
	if (FAILED(hr)) { return hr; }
	if (desc.pIdx)
	{
		hr = CreateIndexBuffer(desc.pIdx, desc.idxSize, desc.idxCount);
		if (FAILED(hr)) { return hr; }
	}

	m_topology = desc.topology;
	return hr;
}


HRESULT DXBuffer::CreateVertexBuffer(const void* pVtx, UINT size, UINT count, bool isWrite)
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	// バッファの初期値を設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pVtx;

	// 頂点バッファの作成
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pVtxBuffer);

	if (SUCCEEDED(hr))
	{
		m_vtxSize = size;
		m_vtxCount = count;
	}
	return hr;
}

HRESULT DXBuffer::CreateIndexBuffer(const void* pIdx, UINT size, UINT count)
{
	// インデックスサイズ読み取り
	switch (size)
	{
	default:
		return E_FAIL;
	case 1:
	case 2:
	case 4:
		break;
	}

	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// バッファの初期データ
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIdx;

	// インデックスバッファ作成
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pIdxBuffer);
	if (SUCCEEDED(hr))
	{
		m_idxSize = size;
		m_idxCount = count;
	}

	return hr;
}

void DXBuffer::Draw(int count)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	UINT stride = m_vtxSize;
	UINT offset = 0;

	pDC->IASetPrimitiveTopology(m_topology);
	pDC->IASetVertexBuffers(0, 1, &m_pVtxBuffer, &stride, &offset);

	// 描画
	if (m_idxCount > 0)
	{
		DXGI_FORMAT format;
		switch (m_idxSize)
		{
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		case 1: format = DXGI_FORMAT_R8_UINT; break;
		}
		pDC->IASetIndexBuffer(m_pIdxBuffer, format, 0);
		pDC->DrawIndexed(count ? count : m_idxCount, 0, 0);
	}
	else
	{
		// 頂点バッファのみで描画
		pDC->Draw(count ? count : m_vtxCount, 0);
	}
}

void DXBuffer::DrawInstancing(int instance, int count)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	UINT stride = m_vtxSize;
	UINT offset = 0;

	pDC->IASetPrimitiveTopology(m_topology);
	pDC->IASetVertexBuffers(0, 1, &m_pVtxBuffer, &stride, &offset);

	// 描画
	if (m_idxCount > 0)
	{
		DXGI_FORMAT format;
		switch (m_idxSize)
		{
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		case 1: format = DXGI_FORMAT_R8_UINT; break;
		}
		//pDC->IASetIndexBuffer(m_pIdxBuffer, format, 0);
		pDC->DrawIndexedInstanced(count ? count : m_idxCount, instance ,0, 0, 0);
	}
	else
	{
		// 頂点バッファのみで描画
		//pDC->Draw(count ? count : m_vtxCount, 0);
		pDC->DrawInstanced(count ? count : m_vtxCount, instance, 0, 0);
	}
}

HRESULT DXBuffer::Write(void* pVtx)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;

	// データコピー
	hr = pDC->Map(m_pVtxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (SUCCEEDED(hr))
	{
		rsize_t size = m_vtxSize * m_vtxCount;
		memcpy_s(mapResource.pData, size, pVtx, size);
		pDC->Unmap(m_pVtxBuffer,0);
	}
	return hr;
}
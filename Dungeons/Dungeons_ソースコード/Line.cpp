#include "Line.h"
#include "Camera.h"
#include "CameraManager.h"

#define LINE_NUM (100)								// 最大線分数
#define LINE_GEOMETORY_NUM (LINE_NUM * 2)			// 最大頂点数

ConstantBuffer* Line::m_pCBuffer = nullptr;
VertexShader* Line::m_pVertex = nullptr;
PixelShader* Line::m_pPixel = nullptr;

Line::Line()
{
	m_LineData.resize(0);
	InitVertexBuffer();
}

Line::~Line()
{

}

HRESULT Line::InitVertexBuffer()
{
	HRESULT hr;
	const float d = 0.5f;

	DXBuffer::Desc desc = {};
	m_LineData.resize(LINE_GEOMETORY_NUM);

	desc.pVtx = m_LineData.data();
	//--------------
	// ライン描画用の頂点バッファ
	//--------------
	desc.vtxSize = sizeof(VERTEX);
	desc.vtxCount = LINE_GEOMETORY_NUM;			// 頂点の数
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	desc.isWrite = true;
	//--------------
	hr = m_Lines.Create(desc);
	if (FAILED(hr)) { return hr; }

	m_LineData.resize(0);
	return hr;
}

// ===線シェーダー初期化===
HRESULT Line::InitLine()
{
	HRESULT hr;

	if (!m_pCBuffer)
	{
		m_pCBuffer = new ConstantBuffer;
		hr = m_pCBuffer->Create(sizeof(XMFLOAT4X4) * 3);
		if (FAILED(hr)) return hr;
	}

	if (!m_pVertex)
	{
		m_pVertex = new VertexShader;
		hr = m_pVertex->Load("data/shader/DebugLineVS.cso");
		if (FAILED(hr)) { return hr; }
	}

	if (!m_pPixel)
	{
		m_pPixel = new PixelShader;
		hr = m_pPixel->Load("data/shader/DebugLinePS.cso");
		if (FAILED(hr)) { return hr; }
	}
	return hr;
}

// 線シェーダー解放
void Line::Fin()
{
	SAFE_DELETE(m_pPixel);
	SAFE_DELETE(m_pVertex);
	SAFE_DELETE(m_pCBuffer);
}

// ワールド変換行列をセット
void Line::SetWorldMatrix(const XMFLOAT4X4& world)
{
	m_World = world;
}

// 色をセット
void Line::SetMaterial(const XMFLOAT4& color)
{
	std::vector<VERTEX>::iterator it = m_LineData.begin();
	while (it != m_LineData.end())
	{
		(*it).color = color;
		++it;
	}
}

// 線情報追加
void Line::AddLine(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 color)
{
	VERTEX vtx;
	// ---始点情報追加---
	vtx.vtx = start;
	vtx.color = color;
	m_LineData.push_back(vtx);
	// ---終点情報追加---
	vtx.vtx = end;
	vtx.color = color;
	m_LineData.push_back(vtx);
}

// 線描画
void Line::DrawLines()
{
	m_Lines.Write(m_LineData.data());

	// 変換行列作成
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&m_World);								// ワールド変換行列
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// ビュー行列
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// プロジェクション行列

	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));

	m_pCBuffer->Write(fMat);		// 定数バッファにセット

	m_pVertex->Bind();				// 頂点シェーダーをセット
	m_pPixel->Bind();				// ピクセルシェーダーをセット
	m_pCBuffer->BindVS(0);			// 頂点シェーダーに渡す
	m_Lines.Draw();					// デバッグ線描画
}
#include "Line.h"
#include "Camera.h"
#include "CameraManager.h"

#define LINE_NUM (100)								// �ő������
#define LINE_GEOMETORY_NUM (LINE_NUM * 2)			// �ő咸�_��

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
	// ���C���`��p�̒��_�o�b�t�@
	//--------------
	desc.vtxSize = sizeof(VERTEX);
	desc.vtxCount = LINE_GEOMETORY_NUM;			// ���_�̐�
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	desc.isWrite = true;
	//--------------
	hr = m_Lines.Create(desc);
	if (FAILED(hr)) { return hr; }

	m_LineData.resize(0);
	return hr;
}

// ===���V�F�[�_�[������===
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

// ���V�F�[�_�[���
void Line::Fin()
{
	SAFE_DELETE(m_pPixel);
	SAFE_DELETE(m_pVertex);
	SAFE_DELETE(m_pCBuffer);
}

// ���[���h�ϊ��s����Z�b�g
void Line::SetWorldMatrix(const XMFLOAT4X4& world)
{
	m_World = world;
}

// �F���Z�b�g
void Line::SetMaterial(const XMFLOAT4& color)
{
	std::vector<VERTEX>::iterator it = m_LineData.begin();
	while (it != m_LineData.end())
	{
		(*it).color = color;
		++it;
	}
}

// �����ǉ�
void Line::AddLine(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 color)
{
	VERTEX vtx;
	// ---�n�_���ǉ�---
	vtx.vtx = start;
	vtx.color = color;
	m_LineData.push_back(vtx);
	// ---�I�_���ǉ�---
	vtx.vtx = end;
	vtx.color = color;
	m_LineData.push_back(vtx);
}

// ���`��
void Line::DrawLines()
{
	m_Lines.Write(m_LineData.data());

	// �ϊ��s��쐬
	DirectX::XMMATRIX mat[3];
	DirectX::XMFLOAT4X4 fMat[3];

	mat[0] = XMLoadFloat4x4(&m_World);								// ���[���h�ϊ��s��
	mat[1] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix());		// �r���[�s��
	mat[2] = XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix());		// �v���W�F�N�V�����s��

	DirectX::XMStoreFloat4x4(&fMat[0], XMMatrixTranspose(mat[0]));
	DirectX::XMStoreFloat4x4(&fMat[1], XMMatrixTranspose(mat[1]));
	DirectX::XMStoreFloat4x4(&fMat[2], XMMatrixTranspose(mat[2]));

	m_pCBuffer->Write(fMat);		// �萔�o�b�t�@�ɃZ�b�g

	m_pVertex->Bind();				// ���_�V�F�[�_�[���Z�b�g
	m_pPixel->Bind();				// �s�N�Z���V�F�[�_�[���Z�b�g
	m_pCBuffer->BindVS(0);			// ���_�V�F�[�_�[�ɓn��
	m_Lines.Draw();					// �f�o�b�O���`��
}
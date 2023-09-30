// 2D�|���S�������N���X���� [Polygon.cpp]
#include "Polygon.h"
#include "CameraManager.h"

// �V�F�[�_�ɓn���l
struct SHADER_GLOBAL {
	XMMATRIX	mWorld;		// ���[���h�ϊ��s��(�]�u�s��)
	XMMATRIX	mView;		// �r���[�ϊ��s��(�]�u�s��)
	XMMATRIX	mProj;		// �ˉe�ϊ��s��(�]�u�s��)
	XMMATRIX	mTex;		// �e�N�X�`���ϊ��s��(�]�u�s��)
};

// �ÓI�����o
ID3D11ShaderResourceView*	CPolygon::m_pTexture;				// �e�N�X�`���ւ̃|�C���^

VERTEX_2D					CPolygon::m_vertexWk[NUM_VERTEX];	// ���_���i�[���[�N

XMFLOAT3					CPolygon::m_vPos;					// �|���S���̈ړ���
XMFLOAT3					CPolygon::m_vAngle;					// �|���S���̉�]��
XMFLOAT3					CPolygon::m_vScale;					// �|���S���̃T�C�Y
XMFLOAT4					CPolygon::m_vColor;					// �|���S���̒��_�J���[
bool						CPolygon::m_bInvalidate;			// ���_�f�[�^�X�V�t���O

XMFLOAT2					CPolygon::m_vPosTexFrame;			// UV���W
XMFLOAT2					CPolygon::m_vSizeTexFrame;			// �e�N�X�`�����o�T�C�Y

ID3D11Buffer*				CPolygon::m_pConstantBuffer;		// �萔�o�b�t�@
ID3D11Buffer*				CPolygon::m_pVertexBuffer;			// ���_�o�b�t�@
ID3D11SamplerState*			CPolygon::m_pSamplerState;			// �e�N�X�`�� �T���v��

VertexShader*				CPolygon::m_VtxShader;				// ���_�V�F�[�_�[
PixelShader*				CPolygon::m_PixShader;				// �s�N�Z���V�F�[�_�[

XMFLOAT4X4					CPolygon::m_mProj;					// �ˉe�ϊ��s��
XMFLOAT4X4					CPolygon::m_mView;					// �r���[�ϊ��s��
XMFLOAT4X4					CPolygon::m_mWorld;					// ���[���h�ϊ��s��
XMFLOAT4X4					CPolygon::m_mTex;					// �e�N�X�`���ϊ��s��

bool						CPolygon::m_bUI;
bool						CPolygon::m_bBillboard;

// ������
HRESULT CPolygon::Init(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;

	// �V�F�[�_������
	m_VtxShader = new VertexShader();								// ---�s�N�Z���V�F�[�_�[�Ǎ�
	if(FAILED(m_VtxShader->Load("data/shader/Vertex2D.cso")))
		MessageBoxW(0, L"hlsl�ǂݍ��ݎ��s", nullptr, MB_OK);

	m_PixShader = new PixelShader();								// ---���_�V�F�[�_�\�Ǎ�
	if (FAILED(m_PixShader->Load("data/shader/Pixel2D.cso")))
		MessageBoxW(0, L"hlsl�ǂݍ��ݎ��s", nullptr, MB_OK);


	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
	if (FAILED(hr)) return hr;

	// �e�N�X�`�� �T���v������
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &m_pSamplerState);
	if (FAILED(hr)) {
		return hr;
	}

	// �ϊ��s�񏉊���
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&m_mProj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&m_mTex, XMMatrixIdentity());
	m_mTex._44 = 0.0f;

	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_bInvalidate = false;

	m_vPosTexFrame = XMFLOAT2(0.0f, 0.0f);
	m_vSizeTexFrame = XMFLOAT2(1.0f, 1.0f);

	m_bUI = true;

	// ���_���̍쐬
	hr = MakeVertex(pDevice);

	return hr;
}

// �I������
void CPolygon::Fin(void)
{
	// �e�N�X�`�� �T���v���̊J��
	SAFE_RELEASE(m_pSamplerState);
	// ���_�o�b�t�@�̉��
	SAFE_RELEASE(m_pVertexBuffer);
	// �萔�o�b�t�@�̉��
	SAFE_RELEASE(m_pConstantBuffer);

	// �s�N�Z���V�F�[�_�[���
	delete m_PixShader;

	// ���_�V�F�[�_�\���
	delete m_VtxShader;
}

// �`��
void CPolygon::Draw(ID3D11DeviceContext* pDeviceContext)
{
	// �g�k
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// ��]
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x),
		XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	// �ړ�
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	// ���[���h �}�g���b�N�X�ɐݒ�
	XMStoreFloat4x4(&m_mWorld, mWorld);

	// �r���{�[�h
	if (m_bBillboard)
	{
		XMFLOAT4X4& mV = CAMERA->GetCurrentCamera()->GetViewMatrix();
		// �r���{�[�h�p���[���h�ϊ�
		m_mWorld._11 = mV._11 * m_vScale.x;
		m_mWorld._12 = mV._21 * m_vScale.x;
		m_mWorld._13 = mV._31 * m_vScale.x;
		m_mWorld._14 = 0.0f;
		m_mWorld._21 = mV._12 * m_vScale.y;
		m_mWorld._22 = mV._22 * m_vScale.y;
		m_mWorld._23 = mV._32 * m_vScale.y;
		m_mWorld._24 = 0.0f;
		m_mWorld._31 = mV._13;
		m_mWorld._32 = mV._23;
		m_mWorld._33 = mV._33;
		m_mWorld._34 = 0.0f;
		// �ʒu�𔽉f
		m_mWorld._41 = m_vPos.x;
		m_mWorld._42 = m_vPos.y;
		m_mWorld._43 = m_vPos.z;
		m_mWorld._44 = 1.0f;
	}

	if (m_pTexture) {
		// �g�k
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// �ړ�
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// �e�N�X�`�� �}�g���b�N�X�ɐݒ�
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// �e�N�X�`������
		m_mTex._44 = 0.0f;
	}

	// ���_�o�b�t�@�X�V
	SetVertex();

	// �V�F�[�_�[�ݒ�
	m_VtxShader->Bind();
	m_PixShader->Bind();

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));

	if (m_bUI == false)
	{
		CameraBase* pCamera = CAMERA->GetCurrentCamera();
		cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetProjMatrix()));
		cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()));
		cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	}

	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// �v���~�e�B�u�`����Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���̕`��
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

// ���_�̍쐬
HRESULT CPolygon::MakeVertex(ID3D11Device* pDevice)
{
	// ���_���W�̐ݒ�
	m_vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_vertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);

	// �g�U���ˌ��̐ݒ�
	m_vertexWk[0].diffuse = m_vColor;
	m_vertexWk[1].diffuse = m_vColor;
	m_vertexWk[2].diffuse = m_vColor;
	m_vertexWk[3].diffuse = m_vColor;

	// �e�N�X�`�����W�̐ݒ�
	m_vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	m_vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	m_vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	m_vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(m_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_vertexWk[0];

	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);

	return hr;
}

// ���_���W�̐ݒ�
void CPolygon::SetVertex(void)
{
	if (m_bInvalidate) {
		//���_�o�b�t�@�̒��g�𖄂߂�
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
		HRESULT hr = S_OK;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		hr = pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr)) {
			// �g�U���ˌ��̐ݒ�
			m_vertexWk[0].diffuse = m_vColor;
			m_vertexWk[1].diffuse = m_vColor;
			m_vertexWk[2].diffuse = m_vColor;
			m_vertexWk[3].diffuse = m_vColor;
			// ���_�f�[�^���㏑��
			memcpy_s(msr.pData, sizeof(m_vertexWk), m_vertexWk, sizeof(m_vertexWk));
			// ���_�f�[�^���A�����b�N����
			pDeviceContext->Unmap(m_pVertexBuffer, 0);
			// �t���O���N���A
			m_bInvalidate = false;
		}
	}
}

// �e�N�X�`���̐ݒ�
void CPolygon::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	m_pTexture = pTexture;
	m_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
}

// �\�����W�̐ݒ�
void CPolygon::SetPos(float fX, float fY)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
}

void CPolygon::SetPos(float fX, float fY, float fZ)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
	m_vPos.z = fZ;
}

void CPolygon::SetPos(XMFLOAT3 vPos)
{
	m_vPos.x = vPos.x;
	m_vPos.y = vPos.y;
	m_vPos.z = vPos.z;
}

// �\���T�C�Y�̐ݒ�
void CPolygon::SetSize(float fScaleX, float fScaleY)
{
	m_vScale.x = fScaleX;
	m_vScale.y = fScaleY;
}

// �\���p�x�̐ݒ�(�P��:�x)
void CPolygon::SetAngle(float fAngle)
{
	m_vAngle.z = fAngle;
}

void CPolygon::SetRotate(XMFLOAT3 vRot)
{
	m_vAngle.x = vRot.x;
	m_vAngle.y = vRot.y;
	m_vAngle.z = vRot.z;
}

// ����e�N�X�`�����W�̐ݒ� (0.0��fU��1.0, 0.0��fV��1.0)
void CPolygon::SetUV(float fU, float fV)
{
	m_vPosTexFrame.x = fU;
	m_vPosTexFrame.y = fV;
}

// �e�N�X�`���t���[���T�C�Y�̐ݒ� (0.0��fWidth��1.0, 0.0��fHeight��1.0)
void CPolygon::SetFrameSize(float fWidth, float fHeight)
{
	m_vSizeTexFrame.x = fWidth;
	m_vSizeTexFrame.y = fHeight;
}

// ���_�J���[�̐ݒ�
void CPolygon::SetColor(float fRed, float fGreen, float fBlue)
{
	SetColor(XMFLOAT3(fRed, fGreen, fBlue));
}
void CPolygon::SetColor(XMFLOAT3 vColor)
{
	if (vColor.x != m_vColor.x || vColor.y != m_vColor.y || vColor.z != m_vColor.z) {
		m_vColor.x = vColor.x;
		m_vColor.y = vColor.y;
		m_vColor.z = vColor.z;
		m_bInvalidate = true;
	}
}

// �s�����x�̐ݒ�
void CPolygon::SetAlpha(float fAlpha)
{
	if (fAlpha != m_vColor.w) {
		m_vColor.w = fAlpha;
		m_bInvalidate = true;
	}
}

// ���_�J���[�̐ݒ�
void CPolygon::SetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
	SetColor(XMFLOAT4(fRed, fGreen, fBlue, fAlpha));
}
void CPolygon::SetColor(XMFLOAT4 vColor)
{
	if (vColor.x != m_vColor.x || vColor.y != m_vColor.y || vColor.z != m_vColor.z || vColor.w != m_vColor.w) {
		m_vColor = vColor;
		m_bInvalidate = true;
	}
}

// UI��2D�̃I�u�W�F�N�g���̐ݒ�
void CPolygon::SetDraw(bool flg)
{
	m_bUI = flg;
}

// �r���{�[�h�L��
void CPolygon::SetBill(bool flg)
{
	m_bBillboard = flg;
}
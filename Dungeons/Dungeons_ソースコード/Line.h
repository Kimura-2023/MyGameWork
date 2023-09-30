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
	static HRESULT InitLine();									// ���`��p�V�F�[�_�[��������		

	HRESULT InitVertexBuffer();									// ���_�o�b�t�@�쐬
	void AddLine(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f});			// ���ǉ�
	void DrawLines();							// ���`��

	void SetWorldMatrix(const XMFLOAT4X4&);
	void SetMaterial(const XMFLOAT4&);

	static void Fin();
private:
	static ConstantBuffer* m_pCBuffer;
	static VertexShader* m_pVertex;
	static  PixelShader* m_pPixel;

	DXBuffer m_Lines;
	std::vector<VERTEX> m_LineData;				// �����
	XMFLOAT4X4 m_World;							// ���[���h�ϊ��s��
};
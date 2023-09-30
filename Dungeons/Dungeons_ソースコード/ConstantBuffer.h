// ---�萔�o�b�t�@�N���X
#pragma once
#include "main.h"

class ConstantBuffer
{
public:
	ConstantBuffer();					// �R���X�g���N�^
	~ConstantBuffer();					// �f�X�g���N�^

	HRESULT Create(UINT size);			// �萔�o�b�t�@����(�f�[�^�T�C�Y(4�o�C�g��؂�))
	void Write(const void* pData);		// �萔�o�b�t�@��������
	void BindVS(UINT slot);				// ���_�V�F�[�_�[��register()�ɒ萔�o�b�t�@���o�C���h
	void BindPS(UINT slot);				// �s�N�Z���V�F�[�_�[��register()�ɒ萔�o�b�t�@���o�C���h

private:
	ID3D11Buffer* m_pBuffer;			// �o�b�t�@
};
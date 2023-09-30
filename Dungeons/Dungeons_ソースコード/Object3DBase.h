#pragma once
// ===�z�u3D�I�u�W�F�N�g�x�[�X�N���X Object3DBase.h
// ---Authot kimura

// ===�C���N���[�h===
#include "main.h"
#include "Object.h"
#include "Model.h"
#include "Calc.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include <memory>

// ===�N���X��`===
class Object3DBase : public Object
{
	// ---�����o�֐�---
public:
	Object3DBase();				// �R���X�g���N�^
	virtual ~Object3DBase();	// �f�X�g���N�^
	virtual void Update();		// �X�V
	virtual void Draw();		// �`��

	void DrawShadow();			// �e�̕`��

	void SetRot(const XMFLOAT3& vRot);					// ��]()
	void SetRot(const Quaternion& qRot);				// ��]�N�H�[�^�j�I�����Z�b�g
	void SetFront(const XMFLOAT3& front);				// �I�u�W�F�N�g�̐��ʕ������Z�b�g
	XMFLOAT3 GetFront();								// �����Ă���������擾
	void SetPS(PixelShader* ps) { m_pPS = ps; }			// �s�N�Z���V�F�[�_�[���Z�b�g
	void SetVS(VertexShader* vs) { m_pVS = vs; }		// ���_�V�F�[�_�\���Z�b�g

	void virtual SetResource(std::string);				// ���\�[�X�̃p�X���Z�b�g(�V�[�����ۑ��̂���)

	Model* GetModel() { return m_Model; }				// �Q�Ƃ��Ă��郂�f���̃|�C���^��Ԃ�
	void UpdateMatrix();								// ���[���h�}�g���N�X�X�V

	virtual void ImGuiWindow() { ; }

protected:
	Model* m_Model;										// ���f���̃|�C���^
	XMFLOAT3 m_vFront;									// ���ʕ���
	VertexShader* m_pVS;								// ���_�V�F�[�_�\
	PixelShader* m_pPS;									// �s�N�Z���V�F�[�_�[

	static std::shared_ptr<ConstantBuffer> m_pWVPBuffer;				// WVP�s��p�萔�o�b�t�@
	static int m_nObjCnt;
private:
	float CalcObjSize(XMFLOAT3*);
};
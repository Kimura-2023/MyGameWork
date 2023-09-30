#pragma once
// ===�h�A�N���X��`===
// �h�A�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "Collision3D.h"

// ===�N���X��`===
class Door : public Object3DBase, Collision3D
{
	// ---�����o�֐�---
public:
	Door();				// �R���X�g���N�^
	~Door();			// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��

	bool IsUpdate();	// �X�V�L��
	void OnCollision(Object*);		// �Փˏ���

	void SetDirection();								// �����ݒ�
	int& GetDirection() { return m_nDirection; }		// �����擾

	XMFLOAT3& GetEntrance() { return m_Entrance; }		// �o���ʒu�擾

	void Open();		// �h�A�J����

	// ---�����o�ϐ�---
private:

	int m_nDirection;		// ����(0,1,2,3)
	XMFLOAT3 m_Entrance;	// �o���ʒu
};
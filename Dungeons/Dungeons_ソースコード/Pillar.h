#pragma once
// ===���N���X��`===
// ���I�u�W�F�N�g�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "Collision3D.h"

// ===�N���X��`===
class Pillar : public Object3DBase, Collision3D
{
	// ---�����o�֐�---
public:
	Pillar();			// �R���X�g���N�^
	~Pillar();			// �f�X�g���N�^

	void Update();		// �X�V����
	void Draw();		// �`�揈��
	bool IsUpdate();	// �X�V�L��
	void OnCollision(Object*);		// �Փˏ���

	// ---�����o�ϐ�---
private:
};
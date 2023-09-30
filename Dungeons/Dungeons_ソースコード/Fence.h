#pragma once
// ===��N���X��`===
// ��N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "SceneManager.h"
#include "Collision3D.h"

// ===�N���X��`===
class Fence : public Object3DBase, Collision3D
{
	// ---�����o�֐�---
public:
	Fence();					// �R���X�g���N�^
	~Fence();					// �f�X�g���N�^

	void Update();				// �X�V����
	void Draw();				// �`�揈��
	bool IsUpdate();			// �X�V�L��
	void OnCollision(Object*);	// �Փˏ���

	// ---�����o�ϐ�---
private:
};
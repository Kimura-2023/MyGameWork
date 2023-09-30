#pragma once
// ===�S�[���I�u�W�F�N�g�N���X��`===
// �S�[���I�u�W�F�N�g�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object3DBase.h"
#include "SceneManager.h"
#include "Collision3D.h"

// ===�N���X��`===
class Goal : public Object3DBase, Collision3D
{
	// ---�����o�֐�---
public:
	Goal();					// �R���X�g���N�^
	~Goal();					// �f�X�g���N�^

	void Update();				// �X�V����
	void Draw();				// �`�揈��
	bool IsUpdate();			// �X�V�L��
	void OnCollision(Object*);	// �Փˏ���

	// ---�����o�ϐ�---
private:
	float m_Rad;				// �㉺�ړ���sin���g�����߂̒l
};
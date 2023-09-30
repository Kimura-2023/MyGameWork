#pragma once
// ===����N���X��`===
// ����̃N���X���`
// Author kimura
// ====================

// ===�C���N���[�h===
#include "main.h"
#include "Collision3D.h"
#include "Object3DBase.h"

// ===�N���X��`===
class Weapon : public Object3DBase,Collision3D
{
	// ---�����o�֐�---
public:
	Weapon(Object3DBase*);				// �R���X�g���N�^(�e�I�u�W�F�N�g�̃|�C���^)
	~Weapon();							// �f�X�g���N�^

	void Update();						// �X�V����
	bool IsUpdate();					// �X�V�L��
	void OnCollision(Object*);			// �Փˏ���
	void SetResource(std::string);		// ���\�[�X�Z�b�g

	void SetAttack(const bool&);		// �ʏ�U���t���O�Z�b�g
	void SetRepel(const bool&);			// ���˃t���O�Z�b�g

	// ---�����o�ϐ�---
private:
	Object3DBase* m_pParent;			// �e�I�u�W�F�N�g(�v���C���[�z��)

	bool m_bAttack;						// �ʏ�U���t���O
	bool m_bRepel;						// ���˃t���O
};
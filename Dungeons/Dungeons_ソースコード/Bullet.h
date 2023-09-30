#pragma once
// ===�e�N���X��`===
// �G�e�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Collision3D.h"
#include "Object3DBase.h"

// ===�N���X��`===
class Bullet : public Object3DBase, Collision3D
{
	// ---�����o�֐�
public:
	Bullet();			// �R���X�g���N�^
	~Bullet();			// �f�X�g���N�^
	
	void Update();		// �X�V
	void Draw();		// �`��
	bool IsUpdate();	// �X�V��
	void OnCollision(Object*);			// �Փˏ���
	void SetRefrect(const XMFLOAT3&);	// ���ˏ�ԃZ�b�g

	bool Fire(const XMFLOAT3& pos, const XMFLOAT3& front, const float& speed);		// �e����

	// ---�����o�ϐ�
private:
	bool m_bRefrect;	// ���˂���Ă��邩�t���O
};
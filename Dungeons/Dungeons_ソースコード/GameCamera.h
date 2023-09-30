#pragma once
// ===�Q�[���J�����N���X��`===
// �Q�[���J�����N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "Camera.h"
#include "Object.h"

// ===�N���X��`===
class GameCamera : public CameraBase
{
	// ---�����o�֐�---
public:
	GameCamera();								// �R���X�g���N�^
	void Init();								// ����������
	void Update();								// �X�V����
	
	void SetTargetObj(Object*);					// �Ǐ]�I�u�W�F�N�g�ݒ�
	Object* GetTargetObj() { return m_Target; }	// �Ǐ]�I�u�W�F�N�g�擾
	void Shake(float second);					// �J�����U��

	// ---�����o�ϐ�---
private:
	Object* m_Target;				// �Ǐ]�I�u�W�F�N�g
	XMFLOAT3 m_vDestPos;			// �ڕW���W
	XMFLOAT3 m_vDestTarget;			// �ڕW�����_
	float m_fShakeFrame;			// �U������
	float m_fShakeRad;				// �U���ʒu(�O�p�֐���-1�`1�̒l�ɔ[�߂�)
};
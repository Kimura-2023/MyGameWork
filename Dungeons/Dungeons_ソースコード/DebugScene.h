// ===�f�o�b�O�p�V�[���N���X��`===
// �f�o�b�O�p�V�[���N���X���`
// ---Author kimura
// =================================
#pragma once


// ===�C���N���[�h===
#include "main.h"
#include "SceneBase.h"

// ===�N���X��`===
class DebugScene : public SceneBase
{
	// ---�����o�֐�---
public:
	DebugScene();			// �R���X�g���N�^
	~DebugScene();			// �f�X�g���N�^
	void Update();			// �X�V����
	void Draw();			// �`�揈��
private:
	void CheckSelectObject();		// �I�u�W�F�N�g�I����ԍX�V

	// ---�����o�ϐ�---
private:
	float m_HitZ;			// �J�[�\�����������Ă����Ԏ�O�̉��s
};
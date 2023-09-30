#pragma once
// ===�X�R�A�G�t�F�N�g�N���X��`===
// �X�R�A�G�t�F�N�g�N���X���`
// Author: Kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include "Object2DBase.h"

// ===�N���X��`===
class ScoreEffect : public Object2DBase
{
	// ---�����o�֐�---
public:
	ScoreEffect();		// �R���X�g���N�^
	~ScoreEffect();		// �f�X�g���N�^

	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	void SetScore(const int& num);		// �X�R�A���Z�b�g

	bool IsUpdate() override { return true; }

	// ---�����o�ϐ�---
private:
	int m_nScore;				// �X�R�A
};
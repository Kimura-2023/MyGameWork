#pragma once
// ===�X�R�A�Ǘ��N���X��`===
// �X�R�A�Ǘ��N���X���`
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Input.h"
#include "SceneManager.h"
#include "Score.h"

// ===�萔�E�}�N����`===
#define SCORE (ScoreManager::Get())					// �C���X�^���X�擾

class Score;
// ===�N���X��`===
class ScoreManager
{
	friend Score;
	// ---�����o�֐�---
public:
	ScoreManager();									// �R���X�g���N�^
	~ScoreManager();								// �f�X�g���N�^

	static ScoreManager* Get();						// �C���X�^���X�擾
	static void Destroy();							// �C���X�^���X�j��
	static void Create();							// �C���X�^���X����

	void Add(const int& num);						// �X�R�A���Z
	void Reset();									// ���݃X�R�A���Z�b�g
	void UpdateHiScore();							// �n�C�X�R�A�X�V

protected:
	const int& GetHiScore();						// �n�C�X�R�A�擾
	const int& GetScore();							// �X�R�A�擾

	// ---�����o�ϐ�---
private:
	static ScoreManager* m_pInstance;				// �C���X�^���X
	int m_nScore;									// ���݃X�R�A
	int m_nHiScore;									// �n�C�X�R�A
};

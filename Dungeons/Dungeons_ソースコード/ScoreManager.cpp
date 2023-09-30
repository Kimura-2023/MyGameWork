// ===�X�R�A�Ǘ��N���X����===
// �X�R�A�Ǘ��N���X������
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "ScoreManager.h"

// ===�萔�E�}�N����`===
#define SCOREFILE ("data/score/hiscore.dat")			// �X�R�A�t�@�C������`

// ===�ÓI�����o===
ScoreManager* ScoreManager::m_pInstance = nullptr;

// ===�R���X�g���N�^===
ScoreManager::ScoreManager() : m_nScore(0),m_nHiScore(0)
{
	// ---�n�C�X�R�A�f�[�^�擾---
	// ---�t�@�C������---
	FILE* fp;
	fopen_s(&fp, SCOREFILE, "rb");			// �t�@�C���I�[�v��(�Ǎ�

	// ---��O����---
	if (!fp)
		return;

	// ---�ǂݍ���---
	fread(&m_nHiScore, sizeof(int), 1, fp);	// �t�@�C������ǂݍ���
	fclose(fp);								// �t�@�C���N���[�Y
}

// ===�f�X�g���N�^===
ScoreManager::~ScoreManager()
{
	UpdateHiScore();
}

// ===����===
void ScoreManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new ScoreManager;		// �C���X�^���X�𐶐�
	}
}

// ===�j��===
void ScoreManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// �C���X�^���X��j��
		m_pInstance = nullptr;				// null���i�[
	}
}

// ===�擾===
ScoreManager* ScoreManager::Get()
{
	return m_pInstance;						// �C���X�^���X��Ԃ�
}

// ===�X�R�A���Z===
void ScoreManager::Add(const int& num)
{
	m_nScore += num;						// �������X�R�A�����Z
}

// ===�X�R�A���Z�b�g===
void ScoreManager::Reset()
{
	m_nScore = 0;
}

// ===�n�C�X�R�A�X�V===
void ScoreManager::UpdateHiScore()
{
	// ---�t�@�C������---
	FILE* fp;
	// ---�n�C�X�R�A�f�[�^��������---
	if (m_nHiScore < m_nScore)				// ���݃X�R�A���n�C�X�R�A�������������ꍇ
	{
		fopen_s(&fp, SCOREFILE, "wb");		// �t�@�C���I�[�v��(��������
		if (fp)
		{
			m_nHiScore = m_nScore;						// �X�R�A�X�V
			fwrite(&m_nScore, sizeof(int), 1, fp);		// �t�@�C���ɏ�������
		}
		fclose(fp);
	}
}

// ===�n�C�X�R�A�擾===
const int& ScoreManager::GetHiScore()
{
	return m_nHiScore;
}

// ===�X�R�A�擾===
const int& ScoreManager::GetScore()
{
	return m_nScore;
}
/*
�쐬��: Kimura Yusaku

�ҏW�ҁF

�X�V����
	 2022/01/21		�t�@�C���쐬
	 2022/01/22		�X�e�[�W�ԍ����Ǘ��ł���悤��
*/

#include "StageSelectManager.h"

StageSelectManager* StageSelectManager::m_pInstance = NULL;

// ������
void StageSelectManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new StageSelectManager;
	}
}

// �I��
void StageSelectManager::Destroy()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

// �擾
StageSelectManager* StageSelectManager::Get()
{
	return m_pInstance;
}

StageSelectManager::StageSelectManager()
{
	m_num = 0;					// �͂��߂͍ŏ��̃X�e�[�W��I��
	for (int i = 0; i < STAGE_SIZE; i++)
	{
		AddStage(false);
	}
}

StageSelectManager::~StageSelectManager()
{
	std::vector<bool>::iterator it;
	it = m_pSelect.begin();
	while (it != m_pSelect.end())
	{
		it = m_pSelect.erase(it);
		++it;
	}
}

void StageSelectManager::AddStage(bool flg)
{
	m_pSelect.push_back(flg);
}

bool StageSelectManager::GetStage(int num)
{
	return m_pSelect[num];
}

void StageSelectManager::SetClear(int num)
{
	m_pSelect[num] = true;
}

void StageSelectManager::SetStageNum(int num)
{
	m_num = num;
}// �X�e�[�W�ԍ��Z�b�g
int StageSelectManager::GetStageNum()
{
	return m_num;
}// �X�e�[�W�ԍ��擾
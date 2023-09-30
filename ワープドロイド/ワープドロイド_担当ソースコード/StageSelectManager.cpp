/*
作成者: Kimura Yusaku

編集者：

更新履歴
	 2022/01/21		ファイル作成
	 2022/01/22		ステージ番号も管理できるように
*/

#include "StageSelectManager.h"

StageSelectManager* StageSelectManager::m_pInstance = NULL;

// 初期化
void StageSelectManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new StageSelectManager;
	}
}

// 終了
void StageSelectManager::Destroy()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

// 取得
StageSelectManager* StageSelectManager::Get()
{
	return m_pInstance;
}

StageSelectManager::StageSelectManager()
{
	m_num = 0;					// はじめは最初のステージを選択
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
}// ステージ番号セット
int StageSelectManager::GetStageNum()
{
	return m_num;
}// ステージ番号取得
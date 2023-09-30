/*
�쐬��: Kimura Yusaku

�ҏW�ҁF

�X�V����
	 2022/01/21		�t�@�C���쐬
*/
#pragma once

#include <vector>

#define SELECT (StageSelectManager::Get())		// �}�l�[�W���擾�u������
#define STAGE_SIZE (4)							// �X�e�[�W��

class StageSelectManager
{
private:
	StageSelectManager();						// �R���X�g���N�^
	~StageSelectManager();						// �f�X�g���N�^

public:
	static void Create();						// ������
	static void Destroy();						// �I��
	static StageSelectManager* Get();			// �擾

	void AddStage(bool);						// �X�e�[�W���ǉ�
	bool GetStage(int);							// �X�e�[�W���擾(�X�e�[�W�̊֐��Ăяo���Ȃ�
	void SetClear(int);
	void SetStageNum(int);						// �X�e�[�W�ԍ��Z�b�g
	int GetStageNum();							// �X�e�[�W�ԍ��擾

private:
	static StageSelectManager* m_pInstance;		// �}�l�[�W��
	std::vector<bool> m_pSelect;				// �X�e�[�W�N���A���
	int m_num;									// ���݃X�e�[�W�ԍ�
};
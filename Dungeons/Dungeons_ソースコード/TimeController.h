#pragma once
#include "main.h"
#include <vector>
#include <functional>
#include <map>

#define RESETINTERVAL (3600)
#define TIME (TimeController::Get())

class TimeController
{
public:
	static void Create();			// ����
	static TimeController* Get();	// �擾
	static void Destroy();			// �j��

	void Update();					// �X�V����
	float GetTime();				// ���Ԏ擾

	void SetTimeUpdateFlg(bool flg = false);

	void SetStop(const float& time) { if(m_stopTime <= 0.0f)m_stopTime = time * 60; }			// �X�V��~���Ԃ��Z�b�g(�b��)
	void SetSlow(const float& time, const int & cut = 2) { if (m_slowTime <= 0.0)m_slowTime = time * 60; m_nCut = cut; }			// �X���[���[�V�������Ԃ��Z�b�g(�b��,����)

	void ResetProcess(void);																	// �Z�b�g���ꂽ�֐���S�ă��Z�b�g
	void ResetProcess(int);																		// �֐��폜(�����w��)
	void SetProcessTimer(const std::function<void(void)>&, float, bool flg = false);			// ���ԍ��֐��Ăяo��

	const bool& GetUpdateFlg() { return m_bEnable; }
private:
	TimeController();
private:
	static TimeController* m_pInstance;

	float m_nowTime;				// �o�߃t���[��
	int m_Second;					// �o�ߎ���(�b)
	bool m_bEnable;					// �X�V�̗L��
	float m_stopTime;				// ��~����
	float m_slowTime;				// �X���[���[�V��������
	int m_nCut;						// �X���[�䗦
	bool m_bPause;					// �|�[�Y

	std::vector <std::tuple<float, std::function<void(void)>, bool>> m_funcList;				// ���s�֐����X�g<<�^�C�}�[,�֐�>,���[�v�w��>
};
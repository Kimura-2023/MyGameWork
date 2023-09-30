#include "TimeController.h"
#include "DebugProc.h"
#include "Input.h"

TimeController* TimeController::m_pInstance = nullptr;

TimeController::TimeController() :m_nowTime(0),m_Second(0),m_stopTime(0),m_slowTime(0),m_nCut(0),m_bPause(false)
{
	m_bEnable = true;
}

void TimeController::Create()
{
	if (!m_pInstance)
		m_pInstance = new TimeController;
}

TimeController* TimeController::Get()
{
	return m_pInstance;
}

void TimeController::Destroy()
{
	delete m_pInstance;
}

void TimeController::Update()
{
	// ---ストップ判定---
	if (m_stopTime > 0.0f)
	{
		--m_stopTime;
		m_bEnable = false;
	}
	else
	{
		m_bEnable = true;
	}

	// ---スローモーション判定---
	if (m_nCut && m_slowTime > 0.0f)
	{
		--m_slowTime;
		if ((int)m_slowTime % m_nCut != 0)
			m_bEnable = false;
		else
			m_bEnable = true;
	}

	if (!m_bEnable)
		return;


	if (CInput::GetKeyTrigger(VK_P))
		m_bPause = !m_bPause;
	m_bEnable = !m_bPause;

	if (m_bPause)
		return;

	m_nowTime = (int)m_nowTime % RESETINTERVAL;
	++m_nowTime;
	m_Second = m_nowTime / 60;

	for (int i = 0; i < m_funcList.size(); ++i)
	{
		if (m_nowTime >= std::get<0>(m_funcList[i]))
		{
			std::get<1>(m_funcList[i])();
			ResetProcess(i);
		}
	}

	CDebugProc::Print("nowFrame:%f\n",m_nowTime);
	CDebugProc::Print("Second:%d\n", m_Second);
}

float TimeController::GetTime()
{
	return m_nowTime;
}

// ===○秒後に関数実行(試験的)===
void TimeController::SetProcessTimer(const std::function<void(void)> &process, float time, bool flg)
{
	float t = (int)(time * 60 + m_nowTime) % RESETINTERVAL;
	m_funcList.push_back(std::make_tuple(t,process,flg));
}

void TimeController::SetTimeUpdateFlg(bool flg)
{
	m_bEnable = flg;
}

void TimeController::ResetProcess()
{
	auto it = m_funcList.begin();
	while (it != m_funcList.end())
	{
		it = m_funcList.erase(it);
		continue;
	}
}

void TimeController::ResetProcess(int i)
{
	m_funcList.erase(m_funcList.begin() + i);
}
/*
作成者:	Tokuda Ryo

更新履歴
	2021/12/15		ファイル作成
	2022/01/10		イメージ更新 更新者：kimura
	2022/01/22		ステージをマネージャーで管理するように変更 更新者 : kimura
	2022/01/27		ステージのロゴ表示 : kimura
*/

#include "SelectScene.h"
#include "input.h"
#include "SceneManager.h"
#include "StageData.h"
#include "Sound.h"
#include "ButtonData.h"
#include "debugproc.h"
#include "Sound.h"
#include "Effect.h"
#include "StageSelectManager.h"

SelectScene::SelectScene()
{
	m_pLogo = new StageLogo;
	m_num = SELECT->GetStageNum();											// 現在ステージ番号

	m_pwhite1 = new Select;
	m_pSelect.push_back(m_pwhite1);
	m_pblack1 = new Stageblack;
	m_pSelect.push_back(m_pblack1);
	m_pwhite2 = new Stagewhite;
	m_pSelect.push_back(m_pwhite2);
	m_pblack2 = new Stageblack2;
	m_pSelect.push_back(m_pblack2);

	m_pPlayer = new StagePlayer;

	m_pAButton = new AButton;
	m_pBButton = new BButton;
	m_pDecision = new DecisionText;
	m_pBack = new BackText;
	m_pStick = new LStick;
	m_pSelectTxt = new SelectText;

	CSound::Play(BGM_001);
	CSound::SetVolume(1.0f);

	for (int i = 0; i < STAGE_SIZE; i++)
	{
		m_pSelect[i]->SetClear(SELECT->GetStage(i));
	}

	for (int i = 1; i < STAGE_SIZE; i++)
	{
		if (!m_pSelect[i - 1]->GetClear())
		{
			m_pSelect[i]->SetColor({ 0.4f,0.4f,0.4f,1.0f });
		}
	}

	m_pPlayer->SetPos(m_pSelect[m_num]->GetPos());
}

SelectScene::~SelectScene()
{
	CSound::Stop(BGM_001);

	delete m_pblack2;
	delete m_pwhite2;
	delete m_pblack1;
	delete m_pwhite1;
	delete m_pSelectTxt;
	delete m_pStick;
	delete m_pBack;
	delete m_pDecision;
	delete m_pBButton;
	delete m_pAButton;
	delete m_pPlayer;
	delete m_pLogo;
}

void SelectScene::Update()
{
	m_num = SelectStage();
	m_pPlayer->SetPos(m_pSelect[m_num]->GetPos());
	m_pLogo->SetNumber(m_num);
	m_pLogo->Draw();

	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, A))
	{
		SELECT->SetStageNum(m_num);												// マネージャーに選択したステージ番号をセット
		switch (m_num)
		{
		case 0:
			SceneManager::GetInstance()->SetScene(SCENE_GAME, STAGEID_TEST0);
			CSound::Play(SE_SELECT);
			return;
			break;
		case 1:
			SceneManager::GetInstance()->SetScene(SCENE_GAME, STAGEID_TEST3);
			CSound::Play(SE_SELECT);
			return;
			break;
		case 2:
			SceneManager::GetInstance()->SetScene(SCENE_GAME, STAGEID_TEST6);
			CSound::Play(SE_SELECT);
			return;
			break;
		case 3:
			SceneManager::GetInstance()->SetScene(SCENE_GAME, STAGEID_TEST9);
			CSound::Play(SE_SELECT);
			return;
			break;
		default:
			break;
		}
	}

	if (GetKeyRelease(VK_B) || GetJoyRelease(JOYSTICKID1, B))
	{
		SceneManager::GetInstance()->SetScene(SCENE_TITLE);
		CSound::Play(SE_SELECT);
		return;
	}

	PrintDebugProc("STAGE[%d]\n", m_num);
	PrintDebugProc("STAGE[%d]\n", m_pSelect[0]->GetClear());
	PrintDebugProc("STAGE[%d]\n", m_pSelect[1]->GetClear());
	PrintDebugProc("STAGE[%d]\n", m_pSelect[2]->GetClear());
	PrintDebugProc("STAGE[%d]\n", m_pSelect[3]->GetClear());
	PrintDebugProc("Playerpos[%f],[%f]\n", m_pPlayer->GetPos().x, m_pPlayer->GetPos().y);
}

int SelectScene::SelectStage()
{
	int next = m_num;

	if (GetStickLTrigger(JOYSTICKID1, DR_RIGHT, 20) || GetKeyRepeat(VK_RIGHT))
	{
		next = (next + 1) % STAGE_SIZE;

		if ((next != 0) && !m_pSelect[next - 1]->GetClear())
		{
			next = m_num;
		}

		if (m_num != next)
		{
			CSound::Play(SE_WARP);				// 効果音再生(ワープ)
			Effect::GetInstance()->Play(EFFECT_WARP, m_pPlayer->GetPos(), 3.0f);
			Effect::GetInstance()->Play(EFFECT_WARPATO, m_pPlayer->GetPos(), 1.0f);
		}
	}
	else if (GetStickLTrigger(JOYSTICKID1, DR_LEFT, 20) || GetKeyRepeat(VK_LEFT))
	{
		next = next - 1;

		if (next < 0)
		{
			next = STAGE_SIZE - 1;
		}

		if ((next != 0) && !m_pSelect[next - 1]->GetClear())
		{
			next = m_num;
		}

		if (m_num != next)
		{
			CSound::Play(SE_WARP);				// 効果音再生(ワープ)
			Effect::GetInstance()->Play(EFFECT_WARP, m_pPlayer->GetPos(), 3.0f);
			Effect::GetInstance()->Play(EFFECT_WARPATO, m_pPlayer->GetPos(), 1.0f);
		}
	}

	return next;
}
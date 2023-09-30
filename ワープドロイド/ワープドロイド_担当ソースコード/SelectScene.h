/*
作成者:	Tokuda Ryo

更新履歴
	2021/12/15		ファイル作成
*/

#pragma once
#include "SceneBase.h"
#include "StagePlayer.h"
#include "Stagewhite2.h"
#include "Stageblack1.h"
#include "Stageblack2.h"
#include "Select.h"
#include "StageLogo.h"
#include "UIAButton.h"
#include "UIDecisionText.h"
#include "UIBButton.h"
#include "UIBackText.h"
#include "UISelectText.h"
#include "UIStick.h"
#include <vector>


class SelectScene : public SceneBase
{
public:
	SelectScene();
	~SelectScene();
	void Update();
	
private:
	int SelectStage();

	//ここにシーンに追加するオブジェクトをメンバ変数で
	int m_num;

	StagePlayer* m_pPlayer;

	StageLogo* m_pLogo;
	std::vector<Select*> m_pSelect;
	Stageblack* m_pblack1;
	Select* m_pwhite1;
	Stageblack2* m_pblack2;
	Stagewhite* m_pwhite2;
	AButton* m_pAButton;
	BButton* m_pBButton;
	LStick* m_pStick;
	DecisionText* m_pDecision;
	BackText* m_pBack;
	SelectText* m_pSelectTxt;
};
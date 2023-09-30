/*
作成者: Kimura Yusaku

編集者：

更新履歴
	 2022/01/10		ファイル作成
*/
#include "Stageblack2.h"

Stageblack2::Stageblack2()
{
	LoadTexture("data/texture/stageblack2.png");
	m_transform.pos = OFLOAT3(45.0f, -10.0f, 0.0f);
}

Stageblack2::~Stageblack2()
{

}
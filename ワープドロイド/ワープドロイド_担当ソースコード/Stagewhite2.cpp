/*
作成者: Kimura Yusaku

編集者：

更新履歴
	 2022/01/10		ファイル作成
*/
#include "Stagewhite2.h"

Stagewhite::Stagewhite()
{
	LoadTexture("data/texture/stagewhite2.png");
	m_transform.pos = OFLOAT3(15.0f, -10.0f, 0.0f);
}

Stagewhite::~Stagewhite()
{

}
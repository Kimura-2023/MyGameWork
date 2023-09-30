/*
作成者: Kimura Yusaku

編集者：

更新履歴
	 2022/01/03		ファイル作成
*/
#pragma once

#include "ObjectBase3D.h"

class StagePlayer : public ObjectBase3D
{
public:
	StagePlayer();
	~StagePlayer();
	void SetPos(OFLOAT3 pos);
	OFLOAT3 GetPos();
private:
};
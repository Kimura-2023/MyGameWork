/*
�쐬��: Kimura Yusaku

�ҏW�ҁF

�X�V����
	 2022/01/03		�t�@�C���쐬
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
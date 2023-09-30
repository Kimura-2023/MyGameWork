/*
�쐬��: Kimura Yusaku

�ҏW�ҁF

�X�V����
	 2022/01/03		�t�@�C���쐬
*/
#include "StagePlayer.h"

StagePlayer::StagePlayer() : ObjectBase3D("data/model/Wait4.fbx")
{
	m_transform.pos = { 0.0f,0.0f,0.0f };
	m_transform.scale = OFLOAT3(0.2f, 0.2f, 0.2f);
	m_transform.rot.y = 0.0f;
	m_isPauseAnimation = false;
}

StagePlayer::~StagePlayer()
{

}

void StagePlayer::SetPos(OFLOAT3 pos)
{
	m_transform.pos = pos;

}

OFLOAT3 StagePlayer::GetPos()
{
	return m_transform.pos;
}
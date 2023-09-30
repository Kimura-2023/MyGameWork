// ===�v���C���[�ҋ@�N���X����===
// �v���C���[�ҋ@�N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "PlayerStay.h"

// ===������===
void PlayerStay::Init(Player* player)
{
	// ---�p�����[�^������---
	player->SetAnimeLoop(true);					// �A�j���[�V�������[�v�ݒ�
	player->SetAnimeRate(0.1f);					// �A�j���[�V�����J�ڗ��ݒ�
	player->SetAtk(false);						// �U����Ԑݒ�
	player->PlayerSetBlendAnime(ANIM_IDLE);		// �A�j���[�V�����J��
}

// ===�X�V����===
void PlayerStay::Update(Player* player)
{
	// ---���x�ݒ�---
	float speed = player->GetSpeed();			// ���x�擾
	XMFLOAT3 vel = player->GetVel();

	speed--;									// ����
	if (speed <= 0.0f)
		speed = 0.0f;							// �ŏ��l�ɔ[�߂�
	vel.x *= speed;
	vel.z *= speed;
	player->SetVel(vel);						// �i�s���x�ݒ�

	// ---�p�����[�^�X�V---
	if (player->GetRate() >= 1.0f)				// �A�j���[�V�����J�ڗ��Ŕ���
	{
		player->SetAnimeLoop(true);				// �A�j���[�V�������[�v�ݒ�
	}

	player->DelStamina(-1);						// �X�^�~�i�X�V
}

// ===�X�V�L��===
bool PlayerStay::IsUpdate(Player* player)
{
	return true;
}
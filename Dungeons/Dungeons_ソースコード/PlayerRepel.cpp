// ===�v���C���[�e���N���X����===
// �v���C���[�e���N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "PlayerRepel.h"
#include "Input.h"
#include "GameManager.h"
#include "DefineData.h"

// ===�v���C���[�U��������===
void PlayerRepel::Init(Player* player)
{
	// ---�A�j���[�V����������---
	player->SetAnimeTime(0.0f);
	player->SetAnimeRate(0.0f);
	player->PlayerSetBlendAnime(ANIM_REPEL);
	player->SetAnimeLoop(false);

	// ---�����v�Z---
	XMFLOAT3 vec;
	if (CInput::GetMouseTrigger(MOUSEBUTTON_R))			// --�}�E�X�̏ꍇ
	{
		vec = GAME->MousePos();							// �}�E�X���W�擾
		XMFLOAT3 pos = player->GetTransform().Pos;		// �v���C���[���W�擾
		vec.x -= pos.x;									// �}�E�X�ւ̃x�N�g�����Z�o
		vec.z -= pos.z;									// �}�E�X�ւ̃x�N�g�����Z�o
		vec.y = 0.0f;									// �����͌v�Z���Ȃ�
	}
	else												// ----�R���g���[���[
	{
		vec = player->GetFront();						// �����Ă�������ɒe��
	}
	Normalize(&vec);								// ���߂��x�N�g���𐳋K��
	player->SetFront(vec);							// �v���C���[�̌������v�Z�����x�N�g���ɂ���

	// ---���x�ݒ�---
	player->SetSpeed(PLAYER_SPEED * 0.1f);				// ���x�Z�b�g
	vec.x *= player->GetSpeed();
	vec.z *= player->GetSpeed();
	player->SetVel(vec);

	// ---����ɒe����Ԃ��Z�b�g---
	Weapon* weapon = player->GetWeapon();
	weapon->SetRepel(true);
}

// ===�v���C���[�e���X�V===
void PlayerRepel::Update(Player* player)
{
	// ---�擾---
	int nowAnime = player->GetAnimeNo();		// ���݃A�j���[�V�����ԍ��擾
	float nowTime = player->GetAnimTime();		// ���݃A�j���[�V�������Ԏ擾
	Model* model = player->GetModel();			// �v���C���[���f���擾
	Weapon* weapon = player->GetWeapon();		// ����擾


	// ---�ҋ@���[�V�����ɐؑ�---
	if (player->GetEndAnime())
	{
		player->SetStrategy(STAY_PLAYER);		// �ҋ@��Ԃɐؑ�
		//weapon->SetRepel(false);				// �e��������
		weapon->SetCol(false);					// ���픻�薳����
		return;
	}

	// ---�e������---
	if (nowTime * model->GetAnimTick(nowAnime) / 60.0f <= model->GetAnimDuration(nowAnime) * 0.7f)			// ���t���[���ȉ��͒e���L��
		weapon->SetCol(true);					// ���픻��L����
	else
		weapon->SetCol(false);					// ���픻�薳����
}

// ===�X�V�L��===
bool PlayerRepel::IsUpdate(Player* player)
{
	return player->GetEndAnime();				// �A�j���[�V�����I������
}
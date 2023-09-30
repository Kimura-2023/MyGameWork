// ===�v���C���[�U���N���X����===
// �v���C���[�U���N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "PlayerAttack.h"
#include "Input.h"
#include "GameManager.h"
#include "DefineData.h"
#include "CameraManager.h"

// ===����������===
void PlayerAttack::Init(Player* player)
{
	if (player->GetStamina() <= 5)	// �X�^�~�i�����ȉ��̏ꍇ�͉������Ȃ�
		return;

	player->SetAnimeTime(0.0f);		// �A�j���[�V�������Ԃ�ݒ�
	player->SetAnimeRate(0.0f);		// �A�j���[�V�����J�ڗ���ݒ�
	player->DelStamina(5);			// �X�^�~�i���X�V

	// ---�����Z�o---
	XMFLOAT3 vec = XMFLOAT3(0.0f,0.0f,0.0f);		// �U������
	if (CInput::GetMouseButton(MOUSEBUTTON_L))		// �}�E�X����
	{
		vec = GAME->MousePos();						// �}�E�X���W���擾
		XMFLOAT3 pos = player->GetTransform().Pos;	// �v���C���[�̍��W���擾
		vec.x -= pos.x;				// ����(x����)���Z�o
		vec.z -= pos.z;				// ����(z����)���Z�o
		vec.y = 0.0f;				// �����͖�������
		Normalize(&vec);			// �x�N�g���𐳋K��
		player->SetFront(vec);		// �v���C���[�̐��ʕ������Z�o�����x�N�g���ɐݒ�
	}
	else											// �R���g���[���[����
	{
		XMFLOAT2 stick = XMFLOAT2(0.0f, 0.0f);		// �X�e�B�b�N���͕���
		stick.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));			// �X�e�B�b�NX�����̓��͒l���擾
		stick.y = -CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));			// �X�e�B�b�NY�����̓��͒l���擾

		// ---�X�e�B�b�N�l�̕␳---
		if (fabs(stick.x) >= DEAD_STICK_X)			// �f�b�h�]�[���𒴂��Ă���Βl�𔽉f
		{
			Normalize(&stick);		// ���͕����𐳋K��(�X�e�B�b�N�̓|����ő��x��ς������ꍇ�͏C��)
			vec.x = stick.x;		// �i�s�����ɃX�e�B�b�N�̒l(X)�𔽉f
		}
		if (fabs(stick.y) >= DEAD_STICK_Y)			// �f�b�h�]�[���𒴂��Ă���Βl�𔽉f
		{
			Normalize(&stick);		// ���͕����𐳋K��(�X�e�B�b�N�̓|����ő��x��ς������ꍇ�͏C��)
			vec.z = stick.y;		// �i�s�����ɃX�e�B�b�N�̒l(Y)�𔽉f
		}
		
		// ---�����̌v�Z---
		XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();	// �J�����̎��_�x�N�g�����擾
		Quaternion q;													// ��]�N�H�[�^�j�I��
		view.y = 0;														// �v���C���[��Y����]���������̂Ŏ��_�x�N�g����Y�̒l�͖�������
		Quaternion::LookRotation(&q, view);								// ���_�����������N�H�[�^�j�I�����Z�o

		Quaternion p(vec);												// ���͕���
		p = -q * p * q;													// ��]�N�H�[�^�j�I���ŋ��ݍ��݁A�i�s�����̕␳������
		vec = XMFLOAT3(p.x, p.y, p.z);									// ���߂�������V�����i�s�����Ƃ���
		XMFLOAT3 input = vec;											// Y�����̑��x�𖳎������l���i�[
		vec.y = player->GetVel().y;										// �i�s���x(Y)���擾

		float speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);		// ���x�����߂�
		if (speed != 0)				// �i�s���x�����������߂��Ă���ꍇ
			player->SetFront(input);				// �v���C���[�̐��ʌ�����i�s�����ɃZ�b�g
		else
			vec = player->GetFront();
	}

	// ---���x�Z�o---
	player->SetSpeed(PLAYER_SPEED * 0.4f);			// ���x�ݒ�
	vec.x *= player->GetSpeed();					// �i�s���x���Z�o(x����)
	vec.z *= player->GetSpeed();					// �i�s���x���Z�o(y����)
	player->SetVel(vec);							// ���x��ݒ�

	// ---�A�j���[�V�����ݒ�---
	player->PlayerSetAnime(ANIM_ATTACK);			// �A�j���[�V�����ݒ�
	player->PlayerSetBlendAnime(-1);				// �J�ڐݒ�
	player->SetAnimeLoop(false);					// ���[�v��Ԑݒ�

	// ---����ݒ�---
	Weapon* weapon = player->GetWeapon();			// ����擾
	weapon->SetAttack(true);						// ����̍U����Ԑݒ�
}

// ===�X�V����===
void PlayerAttack::Update(Player* player)
{
	// ---�擾---
	int nowAnime = player->GetAnimeNo();		// ���݃A�j���[�V�����ԍ��擾
	float nowTime = player->GetAnimTime();		// ���݃A�j���[�V�������Ԏ擾
	Model* model = player->GetModel();			// �v���C���[���f���擾
	Weapon* weapon = player->GetWeapon();		// ����擾

	// ---�ҋ@���[�V�����ɐؑ�---
	if (player->GetEndAnime())					// �A�j���[�V�����I������
	{
		player->SetStrategy(STAY_PLAYER);		// �ҋ@��Ԃɐؑ�
		weapon->SetCol(false);					// ���픻�薳����
		return;
	}

	// ---���픻��ؑ�---
	if (nowTime * model->GetAnimTick(nowAnime) / 60.0f >= model->GetAnimDuration(nowAnime) * 0.3f)				// ���݃A�j���[�V�������ԂŔ���
		weapon->SetCol(true);					// �����蔻��L����
	else
		weapon->SetCol(false);					// �����蔻�薳����

	if (nowTime * model->GetAnimTick(nowAnime) / 60.0f >= model->GetAnimDuration(nowAnime) * 0.7f)				// ���݃A�j���[�V�������ԂŔ���
	{
		if (CInput::GetMouseButton(MOUSEBUTTON_L) || CInput::GetJoyButton(JOYSTICKID1, VB_R))					// �{�^������
			player->SetStrategy(ATTACK2_PLAYER);		// �U��2�i�ڂɑJ��
		if (CInput::GetMouseButton(MOUSEBUTTON_R) || CInput::GetJoyTrigger(JOYSTICKID1, VB_L))
			player->SetStrategy(REPEL_PLAYER);			// �e���ɑJ��
	}
}

// ===�X�V�L��===
bool PlayerAttack::IsUpdate(Player* player)
{
	return player->GetEndAnime();				// �A�j���[�V�����I������
}